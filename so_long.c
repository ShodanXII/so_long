#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH 800
#define HEIGHT 600
#define BORDER_SIZE 20
#define LIL_BRO 50

static mlx_image_t* image_up;
static mlx_image_t* image_down;
static mlx_image_t* image_left;
static mlx_image_t* image_right;
static mlx_image_t* image_standing;
static mlx_image_t* current_image;
  //msa7 kolxi
void delete_all_textures(mlx_texture_t *texture_up, mlx_texture_t *texture_down, mlx_texture_t *texture_left, mlx_texture_t *texture_right, mlx_texture_t *texture_standing)
{
    mlx_delete_texture(texture_up);
    mlx_delete_texture(texture_down);
    mlx_delete_texture(texture_left);
    mlx_delete_texture(texture_right);
    mlx_delete_texture(texture_standing);
}
void handle_key_press(mlx_key_data_t keydata, void* param)
{
    mlx_t* mlx = (mlx_t*)param;
    static bool was_moving = false;
    bool is_moving = false;

    if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
    {
        int32_t x = current_image->instances[0].x;
        int32_t y = current_image->instances[0].y;

        // Hide current image
        current_image->instances[0].enabled = false;

        if (keydata.key == MLX_KEY_UP)
        {
            if (y > 0)
                y -= 12;
            current_image = image_up;
            is_moving = true;
        }
        else if (keydata.key == MLX_KEY_DOWN)
        {
            if (y < HEIGHT - LIL_BRO)
                y += 12;
            current_image = image_down;
            is_moving = true;
        }
        else if (keydata.key == MLX_KEY_LEFT)
        {
            if (x > 0)
                x -= 12;
            current_image = image_left;
            is_moving = true;
        }
        else if (keydata.key == MLX_KEY_RIGHT)
        {
            if (x < WIDTH - LIL_BRO)
                x += 12;
            current_image = image_right;
            is_moving = true;
        }
        else if (keydata.key == MLX_KEY_ESCAPE)
        {
            mlx_close_window(mlx);
            return;
        }

        // Show and update position of new current image
        current_image->instances[0].x = x;
        current_image->instances[0].y = y;
        current_image->instances[0].enabled = true;
    }

    if (!is_moving && was_moving)
    {
        int32_t x = current_image->instances[0].x;
        int32_t y = current_image->instances[0].y;
        current_image->instances[0].enabled = false;
        current_image = image_standing;
        current_image->instances[0].x = x;
        current_image->instances[0].y = y;
        current_image->instances[0].enabled = true;
    }
    was_moving = is_moving;
}

int32_t main(void)
{
    mlx_t* mlx;

    if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
    {
        puts(mlx_strerror(mlx_errno));
        return EXIT_FAILURE;
    }

    // Load textures and create images
    mlx_texture_t* texture_up = mlx_load_png("image/up.png");
    mlx_texture_t* texture_down = mlx_load_png("image/down.png");
    mlx_texture_t* texture_left = mlx_load_png("image/left.png");
    mlx_texture_t* texture_right = mlx_load_png("image/right.png");
    mlx_texture_t* texture_standing = mlx_load_png("image/stand.png");
    
    image_up = mlx_texture_to_image(mlx, texture_up);
    image_down = mlx_texture_to_image(mlx, texture_down);
    image_left = mlx_texture_to_image(mlx, texture_left);
    image_right = mlx_texture_to_image(mlx, texture_right);
    image_standing = mlx_texture_to_image(mlx, texture_standing);

    int32_t center_x = (WIDTH - LIL_BRO) / 2;
    int32_t center_y = (HEIGHT - LIL_BRO) / 2;

    mlx_image_to_window(mlx, image_up, center_x, center_y);
    mlx_image_to_window(mlx, image_down, center_x, center_y);
    mlx_image_to_window(mlx, image_left, center_x, center_y);
    mlx_image_to_window(mlx, image_right, center_x, center_y);
    mlx_image_to_window(mlx, image_standing, center_x, center_y);

    image_up->instances[0].enabled = false;
    image_down->instances[0].enabled = false;
    image_left->instances[0].enabled = false;
    image_right->instances[0].enabled = false;
    
    current_image = image_standing;
    current_image->instances[0].enabled = true;
    delete_all_textures(texture_up, texture_down, texture_left, texture_right, texture_standing);
    mlx_key_hook(mlx, handle_key_press, mlx);
    mlx_loop(mlx);

    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}