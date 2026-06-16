#include "raylib.h"
#include <math.h>

#define WIDTH 800
#define HEIGHT 450
#define DEFAULT_ZOOM 1.0f
#define DEFAULT_PANX 0.0f
#define DEFAULT_PANY 0.0f
#define DEFAULT_ITERS 50

// Lista de colores usados para el mandelbrot
unsigned char colors[16][3] = {
    {66, 30, 15}, {25, 7, 26}, {9, 1, 47}, {4, 4, 73},
    {0, 7, 100}, {12, 44, 138}, {24, 82, 177}, {57, 125, 209},
    {134, 181, 229}, {211, 236, 248}, {241, 233, 191}, {248, 201, 95},
    {255, 170, 0}, {204, 128, 0}, {153, 87, 0}, {106, 52, 3}
};

typedef enum{
    MANDELBROT,
    BURNING_SHIP,
    JULIA,
    TOTAL_FRACTALES
}Fractal_t;

typedef struct {
    double cx;
    double cy;
} JuliaParams;

JuliaParams valoresC[] = {
    {-0.8, 0.156},       // El Dragón
    {-0.70176, -0.3842}, // El Copo de Nieve
    {-0.123, 0.745},     // El Conejo
    {0.285, 0.01}        // Galaxia
};

void mandelbrot(int N, int M, double zoom, double panx, double pany, int iters, double angulo){
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            double x0_base = (x / (double)N - 0.5f) * 3.5f / zoom + panx;
            double y0_base = (y / (double)M - 0.5f) * 2.0f / zoom + pany;

            // Ángulo de rotación
            double cos_a = cos(angulo);
            double sin_a = sin(angulo);

            // Rotación
            double x0 = x0_base * cos_a - y0_base * sin_a;
            double y0 = x0_base * sin_a + y0_base * cos_a;

            double zx = 0.0f, zy = 0.0f;
            int iter = 0;
            while (zx * zx + zy * zy < 4.0f && iter < iters) {
                double temp = zx * zx - zy * zy + x0;
                zy = 2.0f * zx * zy + y0;
                zx = temp;
                iter++;
            }

            if (iter < M) {
                int color_idx = iter % 16;
                Color pixelColor = { 
                    colors[color_idx][0], 
                    colors[color_idx][1], 
                    colors[color_idx][2], 
                    255 // 255 es la opacidad máxima
                };
                
                // Pintamos el píxel directamente en la ventana
                DrawPixel(x, y, pixelColor);
            }
            // Si iter == M, no hacemos nada y dejamos que se vea el fondo negro
        }
    }
}

void burning_ship(int N, int M, double zoom, double panx, double pany, int iters, double angulo){
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            double x0_base = (x / (double)N - 0.5f) * 3.5f / zoom + panx;
            double y0_base = (y / (double)M - 0.5f) * 2.0f / zoom + pany;

            // Ángulo de rotación
            double cos_a = cos(angulo);
            double sin_a = sin(angulo);

            // Rotación
            double x0 = x0_base * cos_a - y0_base * sin_a;
            double y0 = x0_base * sin_a + y0_base * cos_a;

            double zx = 0.0f, zy = 0.0f;
            int iter = 0;
            while (zx * zx + zy * zy < 4.0f && iter < M) {
                // 1. Aplicamos valor absoluto (forzamos que sean positivos)
                zx = fabsf(zx);
                zy = fabsf(zy);
                
                // 2. Calculamos igual que en Mandelbrot, PERO restamos y0 en lugar de sumar
                double temp = zx * zx - zy * zy + x0;
                zy = 2.0f * zx * zy - y0; 
                zx = temp;
                
                iter++;
            }

            if (iter < M) {
                int color_idx = iter % 16;
                Color pixelColor = { 
                    colors[color_idx][0], 
                    colors[color_idx][1], 
                    colors[color_idx][2], 
                    255 // 255 es la opacidad máxima
                };
                
                // Pintamos el píxel directamente en la ventana
                DrawPixel(x, y, pixelColor);
            }
            // Si iter == M, no hacemos nada y dejamos que se vea el fondo negro
        }
    }
}

void julia(int N, int M, double zoom, double panx, double pany, int iters, double angulo, int c_usada) {
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            
            // 1. Calculamos la coordenada del píxel (exactamente igual)
            double x0_base = (x / (double)N - 0.5) * 3.5 / zoom + panx;
            double y0_base = (y / (double)M - 0.5) * 2.0 / zoom + pany;

            // 2. Ángulo de rotación
            double cos_a = cos(angulo);
            double sin_a = sin(angulo);

            // 3. Rotación
            double x0 = x0_base * cos_a - y0_base * sin_a;
            double y0 = x0_base * sin_a + y0_base * cos_a;

            // --- AQUÍ EMPIEZA LA MAGIA DE JULIA ---
            
            // Z empieza siendo la posición del píxel en la pantalla
            double zx = x0;
            double zy = y0;
            int iter = 0;
            
            // La constante C ahora se suma en lugar de x0 e y0
            while (zx * zx + zy * zy < 4.0 && iter < iters) {
                double temp = zx * zx - zy * zy + valoresC[c_usada].cx;
                zy = 2.0 * zx * zy + valoresC[c_usada].cy;
                zx = temp;
                iter++;
            }

            // --- FIN DEL CAMBIO ---

            // Pintamos usando tu misma lógica (corregido iter < iters)
            if (iter < iters) {
                int color_idx = iter % 16;
                Color pixelColor = { 
                    colors[color_idx][0], 
                    colors[color_idx][1], 
                    colors[color_idx][2], 
                    255 
                };
                
                DrawPixel(x, y, pixelColor);
            }
        }
    }
}

int main(void) {
    // 1. Abres la ventana de 800x450 píxeles
    InitWindow(800, 450, "Mi Proyecto Secreto");

    int N = WIDTH;
    int M = HEIGHT;
    int iters = DEFAULT_ITERS;
    double zoom = DEFAULT_ZOOM;
    double panx = DEFAULT_PANX;
    double pany = DEFAULT_PANY;
    double desp = 0.05f;
    double angulo = 0.0f;
    Fractal_t fractal = MANDELBROT;
    double cx = -0.8;
    double cy = 0.156;
    int c_usada = 0;

    // 2. Bucle principal que mantiene la ventana abierta
    while (!WindowShouldClose()) {
        // 1. TECLADO: Mover la vista (usamos IsKeyDown para movimiento continuo)
        // Dividimos la velocidad entre el zoom para que al estar muy cerca no te muevas demasiado rápido
        if (IsKeyDown(KEY_D)) panx += desp / zoom;
        if (IsKeyDown(KEY_A)) panx -= desp / zoom;
        if (IsKeyDown(KEY_W)) pany -= desp / zoom;
        if (IsKeyDown(KEY_S)) pany += desp / zoom;

        if (IsKeyDown(KEY_UP)) desp += 0.01f;
        if (IsKeyDown(KEY_DOWN)) desp -= 0.01f;

        if (IsKeyDown(KEY_Q)) angulo -= 0.05f;
        if (IsKeyDown(KEY_E)) angulo += 0.05f;

        if (IsKeyDown(KEY_C)) {
            int totalValores = sizeof(valoresC) / sizeof(valoresC[0]);
            c_usada = (c_usada + 1) % totalValores;
        }

        if (IsKeyDown(KEY_R)) {
            zoom = DEFAULT_ZOOM;
            panx = DEFAULT_PANX;
            pany = DEFAULT_PANY;
            desp = 0.05f;
            angulo = 0.0f;
            angulo = 0.0f;
        }

        if (IsKeyDown(KEY_N))  {
            fractal = (fractal + 1) % TOTAL_FRACTALES;

            switch(fractal){
                case MANDELBROT:
                    zoom = DEFAULT_ZOOM;
                    panx = DEFAULT_PANX;
                    pany = DEFAULT_PANY;
                    angulo = 0.0f;
                    break;
                case BURNING_SHIP:
                    zoom = 0.8f;
                    //panx = -1.75f;
                    //pany = -0.04f;
                    angulo = 3.14f;
                    break;
                case JULIA:
                    zoom = DEFAULT_ZOOM;
                    panx = DEFAULT_PANX;
                    pany = DEFAULT_PANY;
                    angulo = 0.0f;
                    break;
                default:
                    break;
            }
        }

        double rueda = GetMouseWheelMove(); 
        if (rueda != 0.0f) {
            // Multiplicamos por el propio zoom para que sea suave (acercamiento exponencial)
            zoom += (rueda * 0.1f * zoom); 
            
            // Evitamos que el zoom sea 0 o negativo para no romper las matemáticas ni invertir el fractal
            if (zoom < 0.1f) zoom = 0.1f;
        }

        int iters = 100 + (int)(20.0 * log10(zoom));
        
        BeginDrawing();
            ClearBackground(BLACK); // Pintamos el fondo de negro
            
            switch(fractal){
                case MANDELBROT:
                    mandelbrot(N, M, zoom, panx, pany, iters, angulo);
                    break;
                case BURNING_SHIP:
                    burning_ship(N, M, zoom, panx, pany, iters, angulo);
                    break;
                case JULIA:
                    julia(N, M, zoom, panx, pany, iters, angulo, c_usada);
                    break;
                default:
                    break;
            }
            
            
        EndDrawing();
    }

    // 3. Cierras la ventana al salir
    CloseWindow();
    return 0;
}