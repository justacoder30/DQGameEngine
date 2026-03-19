#include "pch.h"
#include "DQEngine.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Components/TiltedMapComponent.h"

// Shader helpers
GLuint compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint createShaderProgram(const char* vsSource, const char* fsSource) {
    GLuint vertexShader = compileShader(vsSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fsSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// Vertex + Fragment shaders
const char* vertexShaderSource =
R"(#version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource =
R"(#version 330 core
    out vec4 FragColor;
    in vec2 TexCoord;

    uniform sampler2D uTexture;

    void main() {
        FragColor = texture(uTexture, TexCoord);
    }
)";

GLuint LoadTextureWithSDL(const char* filename) {
    GLuint texture;


    SDL_Surface* surface = IMG_Load(filename);
    if (!surface) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        return 0;
    }

    // Force RGBA32 format
    SDL_Surface* conv = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ABGR8888);
    SDL_DestroySurface(surface);


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, conv->w, conv->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, conv->pixels);

    SDL_DestroySurface(conv);
    return texture;
}

int Test()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Request OpenGL 3.3 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("OpenGL Texture Demo", 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return -1;
    }


    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    SDL_GL_SetSwapInterval(-1);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Quad vertices
    //float vertices[] = {
    //    // pos      // texcoords
    //    -0.5f, -0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f,  0.0f, 1.0f
    //};

    float vertices[] = {
        // pos      // texcoords (flipped vertically)
        -0.5f, -0.5f,  0.0f, 1.0f,  // bottom-left
         0.5f, -0.5f,  1.0f, 1.0f,  // bottom-right
         0.5f,  0.5f,  1.0f, 0.0f,  // top-right
        -0.5f,  0.5f,  0.0f, 0.0f   // top-left
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Create shader program
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Load texture with stb_image
    int texW, texH, texChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load("assets/images/tilemap/tileset.png", &texW, &texH, &texChannels, 4); // force RGBA
    if (!data) {
        std::cerr << "Failed to load image!" << std::endl;
        return -1;
    }

    std::cout << "Loaded image with size: " << texW << "x" << texH << " and channels: " << texChannels << std::endl;

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    //texture = LoadTextureWithSDL("assets/images/tilemap/tileset.png");

	// Load texture with SDL_image
    /*SDL_Surface* surface = IMG_Load("assets/images/tilemap/tileset.png");
    if (!surface) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        return 0;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_DestroySurface(surface);*/

	//Delta time variables
    Uint64 startTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = startTime;
    float DeltaTime = 0;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(shaderProgram);
    //Có hiện ra ảnh
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



    SDL_GL_SwapWindow(window);

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        lastTime = startTime;
        startTime = SDL_GetPerformanceCounter();
        DeltaTime = (float)(startTime - lastTime) / SDL_GetPerformanceFrequency();
        
    }

    // Cleanup
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void TestTexture()
{
    SDL_Init(SDL_INIT_VIDEO);

    // yêu cầu OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow(
        "Texture Test",
        1920, 1080,
		//1280, 720,
        SDL_WINDOW_OPENGL
    );

    // Tạo OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to init GLAD\n";
        return;
    }

    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

    Renderer2D::Init();
    Renderer2D::SetViewport(1920, 1080);

    Texture tex = Texture("assets/images/tilemap/tileset.png");
    Texture tex2 = Texture("assets/images/tilemap/Run.png");
	TiltedMapComponent map("assets/demo.tmx");

    Uint64 startTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = startTime;
    float DeltaTime = 0;

    Rect src(0, 0, 384, 448);
    Rect dst(0, 0, 400, 400);

    Rect src2(0, 0, 128, 128);
    Rect dst2(0, 0, 400, 400);

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        lastTime = startTime;
        startTime = SDL_GetPerformanceCounter();
        DeltaTime = (float)(startTime - lastTime) / SDL_GetPerformanceFrequency();
		//std::cout << "DeltaTime: " << DeltaTime << "s\n";

        Renderer2D::BeginScene();

        //Renderer2D::DrawQuad(0, 0, 400.0f, tex);

        //Renderer2D::Draw(tex, src, dst, false, 100.0f);
        Renderer2D::Draw(tex2, src2, dst2);
		map.Draw();

        Renderer2D::EndScene();
        SDL_GL_SwapWindow(window);
    }

}
