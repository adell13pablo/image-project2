#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
//#pragma GCC poison texture_image
#pragma GCC diagnostic ignored "-Wwrite-strings"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#define  BLOCK_SIZE 4
#include <stdio.h>
#include <iostream>
// Image Type
#include <image.h>
#include <cmath> //Library for math functions
// GUI
#include <imgui/imgui.h>
#include <imgui/local.h>  // reset_image, texture_image,
#include <imgui/imgui_glfw.h>

// STL
#include <vector>
#include <string>

// Local functions
static ImGuiWindowFlags static_window();
static void error_callback(int error, const char* description);

/*
Author: Pablo Adell
Date: 01-24-2018
*/
ImVec2 resize(int x, int y)
{
    if (x > 200 && y > 200) {
        float d = (x > y ? x : y) / 200;
        return ImVec2( x/d, y/d );
    }
    return ImVec2(x, y);
}


Image resize_4(Image a)
{
    Image c;
     if (a.texture_loaded) { //If a is loaded but c is not
                c.width = a.width/4;
                c.height = a.height/4;
                c.pixels = (uint8*)malloc(ceil((a.width * a.height * RGBA)/16)); //Allocate the pixels for b
               }

    int len = a.width * a.height * 4;
    int c_len = c.width * c.height * 4;

    printf("Image A width %d height %d total size %d, total size divided by 4 %d\n", a.width, a.height, (a.width * a.height));
        printf("Image C width %d height %d total size %d components of c are %d\n", c.width, c.height, (c.width * c.height), c_len);
    int g = 0 ; //Keeps track of position of c
    printf("Entering the loop on resize_4\n" );
    for(int i = 0; i<len; i+= 4){ //Loop for rows
      for(int j=0; j<a.width*4; j+=32){ //Loop for columns
        //  printf("IN\n");
        for(int k = 0; k<4; k++){
          if((g+k)< c_len){
            c.pixels[g+k] = a.pixels[(a.width*4*i)+j+k];
          }
}


        //  printf("j value is %d\n",j );
        }
      }

    printf("Exiting the loop on resize_4\n");
    return c;

}



//pops up the file list
std::string image_select(int s)
{
    static std::vector<std::string> selected = {"",""};
    ImGui::SameLine();
    ImGui::Text(selected[s] == "" ? "<None>" : selected[s].c_str());

    std::string current_dir;


    int sindex;
    if  (s == 0)
        sindex = fs(selected[s], current_dir, "select 0");

    if  (s == 1)
        sindex = fs(selected[s], current_dir, "select 1");



    if (sindex == -1 )
        return "";
    else
        return current_dir + "/" + selected[s];
}



 void image_embedding(Image a, Image b, uint8* pixels)
{


    int j = 0;
    int len2 = b.width * b.height * 4;

    printf("The width of b is %d and the height is %d , the total size is %d\n", b.width, b.height, (b.width * b.height * 4) );
    int len = a.width * a.height * 4;
    int value = 0;


    // Start hiding bits

    int bitToEmbed = 7; //Position of bit to embed
    int b_pos = 0;
    int pixelToEmbed = b.pixels[b_pos];
    int pixelEmbeded = 0;


    for(int i = 0; i<len; i++){ //Loop that goes through a
    //  printf("Entering the loop\n" );
      pixelEmbeded = a.pixels[i];
    //  printf("Pixel to embed %d\n", pixelEmbeded);
      if(b_pos < len2){ //Check that we dont go further than b.pixels
          int bit = pixelToEmbed >> bitToEmbed;
          bit &=1;
        //  printf("Value of bit is %d\n", bit );
          pixelEmbeded &= (0xFE);
        //  printf("Pixel Embedded after & with FE %d\n", pixelEmbeded );
          pixelEmbeded |= (bit ? 1 : 0);
        //  printf("Pixel after | with bit is %d\n", pixelEmbeded );
          pixels[i] = pixelEmbeded; //Embeding hid pixel within normal pixel
        //  printf("The component in position %d, pixel %d has been embeded\n", b_pos, bitToEmbed );

          if(--bitToEmbed == -1){ //Loop that goes through the bits that from each component of b
            //Get next component of b
          //  printf("The value to embed was %d\n", pixelToEmbed );
            b_pos++; //Get next component
            pixelToEmbed = b.pixels[b_pos]; //Get next component to embed of b
            bitToEmbed = 7; //Restart the position counter
          }
        }else{

      pixels[i] = a.pixels[i]; //Once done with b, fill with a pixels if necessary
}


    }

}


 void image_desembedding( Image c, Image &d, Image a)
{
printf("loop starting\n");
int bitToExtract = 7;
int d_pos = 0;
int desembeded_pixel = 0;
int pixelToDesembed = 0;
int j = 0; //to keep track of the position of c
int len = d.width * d.height * 4;
  printf("The width of d is %d and the height is %d , the total size is %d, we'll need a length of %d\n", d.width, d.height, (d.width * d.height * 4), ((d.width * d.height * 4)*8) );
int len2 = c.width * c.height * 4;
//printf("Value of C length is %d\n", len2 );
for(int k =0; k<(len)*8; k++){ //Multiply by 8 as one byte is 8 bits that we have to combine
  if(d_pos<= len){
  pixelToDesembed = c.pixels[j]; //Get pixels to desembed
//printf("Value to desembed is %d\n", pixelToDesembed );
  pixelToDesembed &= 1; //mask with 1 to get the bit
//printf("After & value of the component is %d\n", pixelToDesembed );
  desembeded_pixel |= (pixelToDesembed << bitToExtract); //shift left to get real value  //OR with actual value to get the new one
//printf("After shifting, value is %d\n", desembeded_pixel );

//printf("Desembeded pixel is %d at position %d\n", desembeded_pixel, bitToExtract );
  if(--bitToExtract == -1){ //when the component value is obtained, add it to the pixel
  //  printf("Value of d new pixel is %d\n", desembeded_pixel );
    d.pixels[d_pos] =desembeded_pixel;
    desembeded_pixel = 0;
    d_pos++;
    bitToExtract = 7;
  }
  j++;
}
}

}






int main()
{
    Image a;
    Image b;
    Image c;
    Image d;
    std::vector<std::string> fnames = {"",""};
    std::string bits_name = "1";

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600,450, "Project 1", NULL, NULL);
    // glfwSetWindowPos(window, -1500, 100);
    glfwMakeContextCurrent(window);

    glewInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    bool slider_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      ImGui_ImplGlfwGL3_NewFrame();

      ImGui::SetNextWindowSize(ImVec2(600,200), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("My project");

      {
        // Image A
        ImGui::BeginGroup();
        {
          if (a.texture_loaded){

                ImGui::Image((void*)a.texture, resize(a.width, a.height));
          }
            if (ImGui::Button("Start"))
                ImGui::OpenPopup("select 0");
            std::string tmp = image_select(0); //opens the file list and assigns to tmp
            if (tmp != "") {
                fnames[0] = tmp;
                reset_image(&a);
                reset_image(&b);

                a.pixels = stbi_load(fnames[0].c_str(), &a.width, &a.height, &a.n, RGBA);
                texture_image(&a);
              }

            if(ImGui::Button("Reset")){
                reset_image(&a);
                reset_image(&b);
                reset_image(&c);
                reset_image(&d);
                free_image(&a);
                free(a.pixels);
                free_image(&b);
                free(b.pixels);
                  free_image(&c);
                free(c.pixels);
                  free_image(&d);
                free(d.pixels);
            }
        }

           ImGui::EndGroup();
           ImGui::SameLine();

            ImGui::BeginGroup();
           {

                if(b.texture_loaded){

                ImGui::Image((void*)b.texture, resize(b.width, b.height));

                }

                if(ImGui::Button("Select watermark"))
                    ImGui::OpenPopup("select 1");


                std::string tmp_2 = image_select(1);
                if(tmp_2 !=""){
                fnames[0] = tmp_2;
                reset_image(&b);
                reset_image(&c);

                b.pixels = stbi_load(fnames[0].c_str(), &b.width, &b.height, &b.n, RGBA);
                texture_image(&b);
              /*  if((b.width * b.height) > 19200){
                b = resize_4(b);
                texture_image(&b);
              }*/
                }

           }
            ImGui::EndGroup();
           //Create Image when process is clicked
        ImGui::BeginGroup();{



                if (ImGui::Button("Embed Image")) { //If the pixels have been allocated, create the image and point to its address for the texture
               reset_image(&c);
               c.width = a.width;
               c.height = a.height;
               c.pixels = (uint8*)malloc(a.width * a.height * RGBA);
               image_embedding(a, b, c.pixels);
                texture_image(&c);
                bits_name = "image";
             }

            if (c.texture_loaded) { //If c has been created, show it on the display
            ImGui::Image((void*)c.texture, resize(c.width, c.height), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,255,0,255));
            if (ImGui::Button("Save")) { //When clicked it saves the picture as prueba.png
              char sname[1000];
              bits_name.append("_embed.png");
              sprintf(sname, bits_name.c_str());
              stbi_write_png(sname, a.width, a.height, 4, c.pixels, 0);
            }
          }
        }

            ImGui::SameLine();

             ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup(); //Image D to display de desembedded watermark
            {




                if (ImGui::Button("Desembed Image")) { //If the pixels have been allocated, create the image and point to its address for the texture
                  reset_image(&d);
                  d.width = b.width;
                  d.height = b.height;
                  d.pixels = (uint8*)malloc((d.width * d.height * RGBA));
               image_desembedding(c, d, a); //Desembed the watermark from the embedded image
                texture_image(&d);




             }

            if (d.texture_loaded) { //If d has been created, show it on the display
            ImGui::Image((void*)d.texture, resize(d.width, d.height), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,255,0,255));
            if (ImGui::Button("Save")) { //When clicked it saves the picture as bit_des.png
              char sname[1000];
              bits_name.append("_desembedding.png");
              sprintf(sname, bits_name.c_str());
              stbi_write_png(sname, d.width, d.height, 4, d.pixels, 0);
            }


          }

             }
     ImGui::EndGroup();




        }




      ImGui::End();

      // Gl Rendering
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui::Render();
      glfwSwapBuffers(window);

    }  //end of while

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    free_image(&a);
    free_image(&b);
    free(a.pixels);
    free(b.pixels);

    return 0;
} //end of main

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

ImGuiWindowFlags static_window()
{
    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = false;
    static bool no_move = false;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = true;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
    return window_flags;
}

/*
       ImGui::BeginGroup();
           {
                if(b.texture_loaded){
                    ImGui::Image((void*)b.texture, resize(b.width, b.height));
                }

                if(ImGui::Button("Select watermark"))
                    ImGui::OpenPopup("select 0");


                std::string tmp_2 = image_select(1);
                if(tmp_2 !=""){
                fnames[0] = tmp_2;
                reset_image(&b);
                reset_image(&c);

                b.pixels = stbi_load(fnames[0].c_str(), &b.width, &b.height, &b.n, RGBA);
                texture_image(&b);
                }

           }
            ImGui::EndGroup();
           //Create Image when process is clicked
        ImGui::BeginGroup();{


               if (a.texture_loaded && b.texture_loaded && !c.texture_loaded) { //If a is loaded but b is not
                c.width = a.width;
                c.height = a.height;
                c.pixels = (uint8*)malloc(a.width * a.height * RGBA); //Allocate the pixels for b
               }

            if(c.texture_loaded != 1){
                if (c.pixels && ImGui::Button("Process jump 1 bit!")) { //If the pixels have been allocated, create the image and point to its address for the texture

                texture_image(&c);
                bits_name = "1";
                                    }


             }

            if (c.texture_loaded) { //If b has been created, show it on the display
            ImGui::Image((void*)b.texture, resize(c.width, c.height), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,255,0,255));
            if (ImGui::Button("Save")) { //When clicked it saves the picture as prueba.png
              char sname[1000];
              bits_name.append("_m.png");
              sprintf(sname, bits_name.c_str());
              stbi_write_png(sname, a.width, a.height, 4, b.pixels, 0);
            }
          }

             }
     ImGui::EndGroup();
*/
