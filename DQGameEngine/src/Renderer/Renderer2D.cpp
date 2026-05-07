#include "pch.h"
#include "Renderer2D.h"
#include "Shader.h"

static const uint32_t MaxQuads = 100000;
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

SDL_Window* Renderer2D::m_Window;
SDL_GLContext Renderer2D::m_Context;
float Renderer2D::m_WindowWidth;
float Renderer2D::m_WindowHeight;
CameraComponent* Renderer2D::s_Camera;

std::vector<RenderCommand> Renderer2D::s_BackgroundQueue;
std::vector<RenderCommand> Renderer2D::s_WorldQueue;
std::vector<RenderCommand> Renderer2D::s_UIQueue;


void Renderer2D::InitWindow(float windowWidth, float windowHeight, const char* title)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	Renderer2D::m_WindowWidth = windowWidth;
	Renderer2D::m_WindowHeight = windowHeight;
    m_Window = SDL_CreateWindow(title, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

    m_Context = SDL_GL_CreateContext(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to init GLAD\n";
    }
}

void Renderer2D::InitRenderer()
{
    uint32_t color = 0xffffffff;

    s_Data.WhiteTexture = new Texture(1, 1, &color);

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

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, TexIndex));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color));
    glEnableVertexAttribArray(3);

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
}

void Renderer2D::Draw(Texture& texture, const Rect& srcrect, const Rect& dstrect, const Color& color, const Flip& flip, const float& angle, const Vector& centerP)
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

    int texIndex = 0;

    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if (s_Data.TextureSlots[i]->GetID() == texture.GetID()) {
            texIndex = i;
            break;
        }
    }

    if (texIndex == 0) {
        if (s_Data.TextureSlotIndex >= MaxTextureSlots) {
            NextBatch();
        }

        texIndex = s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = &texture;
        s_Data.TextureSlotIndex++;
    }
    float texW = (float)texture.GetWidth();
    float texH = (float)texture.GetHeight();

    float u_eps = 0.5f / texW; 
    float v_eps = 0.5f / texH;

    float u0 = (srcrect.x / texW) + u_eps;
    float v1 = 1.0f - (srcrect.y / texH) - v_eps;
    float u1 = ((srcrect.x + srcrect.w) / texW) - u_eps;
    float v0 = 1.0f - ((srcrect.y + srcrect.h) / texH) + v_eps;

    if (flip == Horizontal || flip == Diagonal)
        std::swap(u0, u1);

    if (flip == Vertical || flip == Diagonal)
        std::swap(v0, v1);

    float texCoords[4][2] = {
        { u0, v1 }, // Top-Left
        { u1, v1 }, // Top-Right
        { u1, v0 }, // Bottom-Right
        { u0, v0 }  // Bottom-Left
    };

	Vector center_point = centerP;
    center_point.x = dstrect.x + dstrect.w * centerP.x;
    center_point.y = dstrect.y + dstrect.h * centerP.y;

    //if (center_point != Vector::Zero()) {
    //    center_point.x = dstrect.x + centerP.x;
    //    center_point.y = dstrect.y + centerP.y;
    //}
    //else {
    //    center_point.x = dstrect.x + dstrect.w / 2.0f;
    //    center_point.y = dstrect.y + dstrect.h / 2.0f;
    //}

    //Vector center_point;
    //center_point.x = dstrect.x + dstrect.w * (anchor.x);
    //center_point.y = dstrect.y + dstrect.h * (anchor.y);


    float x = dstrect.x;
    float y = dstrect.y;
    float w = dstrect.w;
    float h = dstrect.h;

    Vector pivot;
    pivot.x = dstrect.w * centerP.x;
    pivot.y = dstrect.h * centerP.y;

    float localVertices[4][2] = {
        { -pivot.x,         -pivot.y },
        {  w - pivot.x,     -pivot.y },
        {  w - pivot.x,      h - pivot.y },
        { -pivot.x,          h - pivot.y }
    };

    //float localVertices[4][2] = {
    //    { -dstrect.w / 2.0f, -dstrect.h / 2.0f },
    //    {  dstrect.w / 2.0f, -dstrect.h / 2.0f },
    //    {  dstrect.w / 2.0f,  dstrect.h / 2.0f },
    //    { -dstrect.w / 2.0f,  dstrect.h / 2.0f }
    //};

    float rad = glm::radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);

    for (int i = 0; i < 4; i++)
    {
        float rotatedX = localVertices[i][0] * cosA - localVertices[i][1] * sinA;
        float rotatedY = localVertices[i][0] * sinA + localVertices[i][1] * cosA;

        s_Data.VertexBufferPtr->Position[0] = rotatedX + center_point.x;
        s_Data.VertexBufferPtr->Position[1] = rotatedY + center_point.y;
        s_Data.VertexBufferPtr->Position[2] = 0.0f;

        s_Data.VertexBufferPtr->TexCoord[0] = texCoords[i][0];
        s_Data.VertexBufferPtr->TexCoord[1] = texCoords[i][1];
        s_Data.VertexBufferPtr->TexIndex = texIndex;

        s_Data.VertexBufferPtr->Color[0] = color.r;
        s_Data.VertexBufferPtr->Color[1] = color.g;
        s_Data.VertexBufferPtr->Color[2] = color.b;
        s_Data.VertexBufferPtr->Color[3] = color.a;

        s_Data.VertexBufferPtr++;
    }
    s_Data.IndexCount += 6;
}

void Renderer2D::DrawRect(const Rect& rect)
{
    Texture& tex = *s_Data.WhiteTexture;

    if (s_Data.IndexCount + 6 > MaxIndices)
        NextBatch();

    uint32_t currentVertexCount = (uint32_t)(s_Data.VertexBufferPtr - s_Data.VertexBufferBase);

    if (currentVertexCount + 4 > MaxVertices)
        NextBatch();

    
    int texIndex = 0; 

    float texCoords[4][2] = {
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f}
    };

    float x = rect.x;
    float y = rect.y;
    float w = rect.w;
    float h = rect.h;

    float vertices[4][2] = {
        {x,     y},
        {x + w, y},
        {x + w, y + h},
        {x,     y + h}
    };

    for (int i = 0; i < 4; i++)
    {
        s_Data.VertexBufferPtr->Position[0] = vertices[i][0];
        s_Data.VertexBufferPtr->Position[1] = vertices[i][1];
        s_Data.VertexBufferPtr->Position[2] = 0.0f;

        s_Data.VertexBufferPtr->TexCoord[0] = texCoords[i][0];
        s_Data.VertexBufferPtr->TexCoord[1] = texCoords[i][1];

        s_Data.VertexBufferPtr->TexIndex = texIndex;

        s_Data.VertexBufferPtr->Color[0] = 0.0f;
        s_Data.VertexBufferPtr->Color[1] = 0.0f;
        s_Data.VertexBufferPtr->Color[2] = 1.0f;
        s_Data.VertexBufferPtr->Color[3] = 1.0f;

        s_Data.VertexBufferPtr++;
    }

    s_Data.IndexCount += 6;

    //memcpy(s_Data.VertexBufferPtr->Color, color, sizeof(float) * 4);
}

void Renderer2D::DrawRectOutline(const Rect& rect, float thickness)
{
    // Top
	Renderer2D::SubmitRect(Rect(rect.x, rect.y, rect.w, thickness), RenderLayer::World);

    // Bottom
	Renderer2D::SubmitRect(Rect(rect.x, rect.y + rect.h - thickness, rect.w, thickness), RenderLayer::World);

    // Left
	Renderer2D::SubmitRect(Rect(rect.x, rect.y, thickness, rect.h), RenderLayer::World);

    // Right
	Renderer2D::SubmitRect(Rect(rect.x + rect.w - thickness, rect.y, thickness, rect.h), RenderLayer::World);
}

void Renderer2D::SetMatrix(const glm::mat4& viewProj)
{
    s_Data.ShaderPtr->SetMat4("u_ViewProjection", viewProj);
}

void Renderer2D::Submit(const RenderCommand& cmd)
{
    switch (cmd.layer)
    {
        case RenderLayer::Background: 
            s_BackgroundQueue.push_back(cmd);
            break;
        case RenderLayer::World:
            s_WorldQueue.push_back(cmd);
            break;
        case RenderLayer::UI: 
            s_UIQueue.push_back(cmd);
            break;
    }
}

void Renderer2D::SubmitRect(const Rect& rect, RenderLayer layer)
{
    RenderCommand cmd;
    cmd.type = CommandType::Rect;
    cmd.dst = rect;

    switch (layer)
    {
    case RenderLayer::Background:
        s_BackgroundQueue.push_back(cmd);
        break;
    case RenderLayer::World:
        s_WorldQueue.push_back(cmd);
        break;
    case RenderLayer::UI:
        s_UIQueue.push_back(cmd);
        break;
    }
}

void Renderer2D::FlushLayer(const RenderLayer& layer)
{
    auto& queue = GetQueue(layer);

    for (auto& cmd : queue)
    {
        if (cmd.type == CommandType::Sprite)
        {
            Draw(*cmd.texture, cmd.src, cmd.dst, cmd.color, cmd.flip, cmd.angle, cmd.center);
        }
        else if (cmd.type == CommandType::Rect)
        {
            DrawRect(cmd.dst); 
        }
    }

}

void Renderer2D::ClearCommandQueue()
{
    s_BackgroundQueue.clear();
    s_WorldQueue.clear();
    s_UIQueue.clear();
}

std::vector<RenderCommand>& Renderer2D::GetQueue(const RenderLayer& layer)
{
    switch (layer)
    {
        case RenderLayer::Background:
		    return s_BackgroundQueue;

        case RenderLayer::World:
		    return s_WorldQueue;

        case RenderLayer::UI:
		    return s_UIQueue;
    }
}

void Renderer2D::EndScene()
{
    SDL_GL_SwapWindow(m_Window);
}

void Renderer2D::Destroy()
{
    SDL_GL_DestroyContext(m_Context);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void Renderer2D::SetViewport(float gameWidth, float gameHeight)
{
	s_Camera = new CameraComponent(gameWidth, gameHeight);
}

void Renderer2D::Flush()
{
    GLsizeiptr size = (uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase;

    if (size == 0)
        return;

    glBindVertexArray(s_Data.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.VertexBufferBase);

    s_Data.ShaderPtr->Bind();
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
    {
        s_Data.TextureSlots[i]->Bind(i);
    }

    glDrawElements( GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
}

void Renderer2D::StartBatch(CameraComponent* camera)
{
    s_Data.IndexCount = 0;
    s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
    s_Data.TextureSlotIndex = 1;

    if(camera) SetMatrix(camera->GetViewProjection());
    else SetMatrix(s_Camera->GetBackdropMatrix());
}

void Renderer2D::EndBatch()
{
    Flush();
}

void Renderer2D::NextBatch()
{
	EndBatch();
    StartBatch();
}
