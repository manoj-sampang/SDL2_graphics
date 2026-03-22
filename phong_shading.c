/*
 * Phong Shading Visualization with SDL2
 * =====================================
 * Complete working example that renders a sphere with Phong illumination
 * 
 * Compile with:
 * gcc phong_demo.c -o phong_demo -lSDL2 -lm
 * 
 * Run:
 * ./phong_demo
 */

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SPHERE_RADIUS 200

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float ka;        // Ambient coefficient
    float kd;        // Diffuse coefficient
    float ks;        // Specular coefficient
    float shininess; // Shininess (n value)
    Vector3 color;   // Base color
} Material;

// ============================================================================
// VECTOR OPERATIONS
// ============================================================================

// Normalize vector to unit length
Vector3 normalize(Vector3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len < 0.0001f) return (Vector3){0, 0, 1}; // Avoid division by zero
    return (Vector3){v.x / len, v.y / len, v.z / len};
}

// Dot product
float dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Reflect vector L across normal N
Vector3 reflect(Vector3 L, Vector3 N) {
    float d = 2.0f * dot(L, N);
    return (Vector3){
        d * N.x - L.x,
        d * N.y - L.y,
        d * N.z - L.z
    };
}

// Subtract two vectors
Vector3 subtract(Vector3 a, Vector3 b) {
    return (Vector3){a.x - b.x, a.y - b.y, a.z - b.z};
}

// ============================================================================
// PHONG ILLUMINATION MODEL
// ============================================================================

Vector3 phong_lighting(
    Vector3 position,
    Vector3 normal,
    Vector3 light_pos,
    Vector3 view_pos,
    Material mat,
    Vector3 light_color,
    float light_intensity
) {
    // Normalize all vectors
    Vector3 N = normalize(normal);
    Vector3 L = normalize(subtract(light_pos, position));
    Vector3 V = normalize(subtract(view_pos, position));
    
    // 1. AMBIENT COMPONENT
    Vector3 ambient = {
        mat.ka * mat.color.x * 0.3f,  // 0.3 = ambient light intensity
        mat.ka * mat.color.y * 0.3f,
        mat.ka * mat.color.z * 0.3f
    };
    
    // 2. DIFFUSE COMPONENT
    float diff = fmaxf(0.0f, dot(N, L));
    Vector3 diffuse = {
        mat.kd * diff * mat.color.x * light_color.x * light_intensity,
        mat.kd * diff * mat.color.y * light_color.y * light_intensity,
        mat.kd * diff * mat.color.z * light_color.z * light_intensity
    };
    
    // 3. SPECULAR COMPONENT
    Vector3 R = reflect((Vector3){-L.x, -L.y, -L.z}, N);
    float spec = powf(fmaxf(0.0f, dot(R, V)), mat.shininess);
    Vector3 specular = {
        mat.ks * spec * light_color.x * light_intensity,
        mat.ks * spec * light_color.y * light_intensity,
        mat.ks * spec * light_color.z * light_intensity
    };
    
    // COMBINE ALL THREE
    Vector3 result = {
        ambient.x + diffuse.x + specular.x,
        ambient.y + diffuse.y + specular.y,
        ambient.z + diffuse.z + specular.z
    };
    
    // Clamp to valid range [0, 1]
    result.x = fminf(1.0f, fmaxf(0.0f, result.x));
    result.y = fminf(1.0f, fmaxf(0.0f, result.y));
    result.z = fminf(1.0f, fmaxf(0.0f, result.z));
    
    return result;
}

// ============================================================================
// SPHERE RENDERING
// ============================================================================

// Check if point (x, y) is inside the sphere
int is_inside_sphere(int x, int y, int cx, int cy, int radius) {
    int dx = x - cx;
    int dy = y - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

// Calculate z-coordinate and normal for a point on sphere surface
void get_sphere_point(int x, int y, int cx, int cy, int radius, Vector3* pos, Vector3* normal) {
    // Convert screen coordinates to sphere-local coordinates
    float sx = (float)(x - cx);
    float sy = (float)(y - cy);
    
    // Calculate z using sphere equation: x² + y² + z² = r²
    float z_squared = radius * radius - sx * sx - sy * sy;
    float sz = (z_squared > 0) ? sqrtf(z_squared) : 0;
    
    // Position in 3D space
    pos->x = sx;
    pos->y = sy;
    pos->z = sz;
    
    // Normal for a sphere is simply the normalized position vector
    *normal = normalize(*pos);
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    
    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Phong Shading Demo - Move mouse to move light!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Setup scene
    int sphere_cx = SCREEN_WIDTH / 2;
    int sphere_cy = SCREEN_HEIGHT / 2;
    
    // Camera position (viewer)
    Vector3 view_pos = {0, 0, 500};
    
    // Light position (starts at center, mouse will control it)
    Vector3 light_pos = {0, 0, 300};
    Vector3 light_color = {1.0f, 1.0f, 1.0f};  // White light
    float light_intensity = 1.0f;
    
    // Material properties - try changing these!
    Material material = {
        .ka = 0.2f,           // Ambient coefficient
        .kd = 0.7f,           // Diffuse coefficient
        .ks = 0.8f,           // Specular coefficient
        .shininess = 32.0f,   // Shininess (try 5, 32, 100, 200)
        .color = {0.2f, 0.5f, 0.8f}  // Blue color (RGB)
    };
    
    printf("=======================================================\n");
    printf("PHONG SHADING DEMO\n");
    printf("=======================================================\n");
    printf("Move your mouse to move the light source!\n");
    printf("Watch how the specular highlight follows the light.\n");
    printf("\nMaterial settings:\n");
    printf("  Ambient (ka):  %.2f\n", material.ka);
    printf("  Diffuse (kd):  %.2f\n", material.kd);
    printf("  Specular (ks): %.2f\n", material.ks);
    printf("  Shininess (n): %.0f\n", material.shininess);
    printf("\nPress ESC or close window to quit.\n");
    printf("=======================================================\n");
    
    // Main loop
    int running = 1;
    SDL_Event event;
    
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                // Update light position based on mouse
                light_pos.x = event.motion.x - sphere_cx;
                light_pos.y = event.motion.y - sphere_cy;
                // Keep z constant for simplicity
            }
        }
        
        // Clear screen (black background)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Render sphere with Phong shading
        for (int y = sphere_cy - SPHERE_RADIUS; y <= sphere_cy + SPHERE_RADIUS; y++) {
            for (int x = sphere_cx - SPHERE_RADIUS; x <= sphere_cx + SPHERE_RADIUS; x++) {
                // Check if point is inside sphere
                if (is_inside_sphere(x, y, sphere_cx, sphere_cy, SPHERE_RADIUS)) {
                    // Get 3D position and normal for this point
                    Vector3 position, normal;
                    get_sphere_point(x, y, sphere_cx, sphere_cy, SPHERE_RADIUS, &position, &normal);
                    
                    // Calculate Phong lighting
                    Vector3 color = phong_lighting(
                        position,
                        normal,
                        light_pos,
                        view_pos,
                        material,
                        light_color,
                        light_intensity
                    );
                    
                    // Convert to RGB (0-255)
                    Uint8 r = (Uint8)(color.x * 255);
                    Uint8 g = (Uint8)(color.y * 255);
                    Uint8 b = (Uint8)(color.z * 255);
                    
                    // Draw pixel
                    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
        
        // Draw light position indicator (small circle)
        int light_screen_x = (int)light_pos.x + sphere_cx;
        int light_screen_y = (int)light_pos.y + sphere_cy;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow
        for (int dy = -5; dy <= 5; dy++) {
            for (int dx = -5; dx <= 5; dx++) {
                if (dx*dx + dy*dy <= 25) {
                    SDL_RenderDrawPoint(renderer, light_screen_x + dx, light_screen_y + dy);
                }
            }
        }
        
        // Present
        SDL_RenderPresent(renderer);
        
        // Small delay to prevent CPU overuse
        SDL_Delay(16);  // ~60 FPS
    }
    
    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    printf("\nThank you for trying the Phong Shading demo!\n");
    
    return 0;
}
