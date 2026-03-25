#include "pch.h"
#include "Renderer2D.h"
#include "Shader.h"

static const uint32_t MaxQuads = 10000;
static const uint32_t MaxVertices = MaxQuads * 4;
static const uint32_t MaxIndices = MaxQuads * 6;
static const uint32_t MaxTextureSlots = 32;

struct RendererData
{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    uint32_t IndexCount = 0;

    QuadVertex* VertexBufferBase;
    QuadVertex* VertexBufferPtr;

    Texture* TextureSlots[MaxTextureSlots];
    uint32_t TextureSlotIndex = 1;

    Shader* ShaderPtr;

    Texture* WhiteTexture;
};

static RendererData s_Data;

void Renderer2D::Init()
{
    uint32_t white = 0xffffffff;

    s_Data.WhiteTexture = new Texture(1, 1, &white);

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    glGenVertexArrays(1, &s_Data.VAO);
    glBindVertexArray(s_Data.VAO);

    glGenBuffers(1, &s_Data.VBO);

    glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);

    glBufferData( GL_ARRAY_BUFFER, MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW );

    glGenBuffers(1, &s_Data.EBO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    uint32_t* indices = new uint32_t[MaxIndices];

    uint32_t offset = 0;

    for (uint32_t i = 0; i < MaxIndices; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.EBO);

    glBufferData( GL_ELEMENT_ARRAY_BUFFER, MaxIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW );

    delete[] indices;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, Position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, TexCoord));
    glEnableVertexAttribArray(1);

    //glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(5 * sizeof(float)));
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(QuadVertex), (void*)offsetof(QuadVertex, TexIndex));
    glEnableVertexAttribArray(2);

    s_Data.VertexBufferBase = new QuadVertex[MaxVertices];

    s_Data.ShaderPtr = new Shader(
        "../DQGameEngine/assets/shaders/vertex.glsl",
        "../DQGameEngine/assets/shaders/fragment.glsl"
    );

    int samplers[32];

    for (int i = 0; i < 32; i++)
        samplers[i] = i;

    s_Data.ShaderPtr->Bind();
    s_Data.ShaderPtr->SetIntArray("u_Textures", samplers, 32);
}

void Renderer2D::BeginScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    StartBatch();
}

void Renderer2D::Draw(Texture& texture, Rect srcrect, Rect dstrect, bool flip, float angle, Vector centerP)
{
    if (s_Data.IndexCount + 6 > MaxIndices)
    {
        NextBatch();
    }

    uint32_t currentVertexCount = (uint32_t)(s_Data.VertexBufferPtr - s_Data.VertexBufferBase);

    if (currentVertexCount + 4 > MaxVertices)
    {
        NextBatch();
    }

    // --- 1. Quản lý Texture Slot (giống hàm cũ) ---
    int texIndex = -1;

    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if (s_Data.TextureSlots[i]->GetID() == texture.GetID()) {
            texIndex = i;
            break;
        }
    }

    if (texIndex == -1) {
        if (s_Data.TextureSlotIndex >= MaxTextureSlots) {
            NextBatch();
        }

        texIndex = s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = &texture;
        s_Data.TextureSlotIndex++;
    }

    // --- 2. Tính toán tọa độ Texture (UV Mapping) ---
    // Chuyển đổi từ pixel (srcrect) sang dải 0.0 -> 1.0 của OpenGL
    float texW = (float)texture.GetWidth();
    float texH = (float)texture.GetHeight();

    float u_eps = 0.5f / texW; // Khoảng eps tính bằng 0.5 pixel
    float v_eps = 0.5f / texH;

    // Tính toán lại UV cho chuẩn
    //float u0 = srcrect.x / texW;
    //float v1 = 1.0f - (srcrect.y / texH); // Đảo ngược trục V
    //float u1 = (srcrect.x + srcrect.w) / texW;
    //float v0 = 1.0f - ((srcrect.y + srcrect.h) / texH);

    float u0 = (srcrect.x / texW) + u_eps;
    float v1 = 1.0f - (srcrect.y / texH) - v_eps;
    float u1 = ((srcrect.x + srcrect.w) / texW) - u_eps;
    float v0 = 1.0f - ((srcrect.y + srcrect.h) / texH) + v_eps;

    if (flip) std::swap(u0, u1);

    float texCoords[4][2] = {
        { u0, v1 }, // Top-Left
        { u1, v1 }, // Top-Right
        { u1, v0 }, // Bottom-Right
        { u0, v0 }  // Bottom-Left
    };

    // --- 3. Tính toán vị trí đỉnh và Xoay (Transformations) ---
    // Tâm của vật thể (để xoay quanh tâm)
    if (centerP != Vector::Zero()) {
        centerP.x = dstrect.x + centerP.x;
        centerP.y = dstrect.y + centerP.y;
    }
    else {
        centerP.x = dstrect.x + dstrect.w / 2.0f;
        centerP.y = dstrect.y + dstrect.h / 2.0f;
    }

    // 4 đỉnh tương đối so với tâm (Local Space)
    float localVertices[4][2] = {
        { -dstrect.w / 2.0f, -dstrect.h / 2.0f },
        {  dstrect.w / 2.0f, -dstrect.h / 2.0f },
        {  dstrect.w / 2.0f,  dstrect.h / 2.0f },
        { -dstrect.w / 2.0f,  dstrect.h / 2.0f }
    };

    float rad = glm::radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);

    for (int i = 0; i < 4; i++)
    {
        // Xoay đỉnh bằng công thức Rotation Matrix
        float rotatedX = localVertices[i][0] * cosA - localVertices[i][1] * sinA;
        float rotatedY = localVertices[i][0] * sinA + localVertices[i][1] * cosA;

        // Đưa về vị trí thế giới (World Space) và lưu vào Buffer
        s_Data.VertexBufferPtr->Position[0] = rotatedX + centerP.x;
        s_Data.VertexBufferPtr->Position[1] = rotatedY + centerP.y;
        s_Data.VertexBufferPtr->Position[2] = 0.0f;

        s_Data.VertexBufferPtr->TexCoord[0] = texCoords[i][0];
        s_Data.VertexBufferPtr->TexCoord[1] = texCoords[i][1];
        s_Data.VertexBufferPtr->TexIndex = texIndex;

        s_Data.VertexBufferPtr++;
    }

    s_Data.IndexCount += 6;
}

void Renderer2D::EndScene()
{
    GLsizeiptr size = (uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase;

    if (size == 0)
        return; 
    glBindVertexArray(s_Data.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.VertexBufferBase);

    Flush();
}

void Renderer2D::SetViewport(float width, float height)
{
    // Tạo ma trận đưa hệ tọa độ về: trái=0, phải=width, dưới=height, trên=0
    // Điều này giúp bạn vẽ DrawQuad(100, 100, 50, tex) sẽ ra đúng 50 pixel
    glm::mat4 proj = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

    s_Data.ShaderPtr->Bind();
    // Truyền ma trận vào Shader (bạn cần viết thêm hàm SetMat4 trong Shader class)
    s_Data.ShaderPtr->SetMat4("u_ViewProjection", proj);

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void Renderer2D::Flush()
{
    s_Data.ShaderPtr->Bind();
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
    {
        s_Data.TextureSlots[i]->Bind(i);
    }

    glBindVertexArray(s_Data.VAO);

    glDrawElements( GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
}

void Renderer2D::StartBatch()
{
    s_Data.IndexCount = 0;
    s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
    s_Data.TextureSlotIndex = 1;

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;
}

void Renderer2D::NextBatch()
{
    EndScene();
    StartBatch();
}
