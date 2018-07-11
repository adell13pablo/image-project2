#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
//#pragma GCC poison texture_image
#pragma GCC diagnostic ignored "-Wwrite-strings"
//OpenCV libraries
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>


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

using namespace cv;

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




void swap_faces(Image a, uint8* pixels, Mat original){
  if(original.empty()) printf("Error abriendo el archivo\n");
  printf("Testing OpenCV\n");
  printf("Loading Haar-Cascade\n");
  CascadeClassifier faceDetector;
  bool loaded1 = faceDetector.load("data/haarcascade_frontalface_default.xml");
  printf("Training Loaded? %d\n", loaded1 );


  std::vector<Rect> faces; //This will store the values where the faces are at (x,y,w,h), as well as the size of them
  cvtColor(original, original, COLOR_BGR2GRAY); //Change color of image from rgb to greyscale
  equalizeHist(original, original); //Equialize the histogram of the image so the contrast is deeper and we can detect better the faces

  faceDetector.detectMultiScale(original, faces, 1.30, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30)); //Detect the faces from src, storing it into a vector with max size

  std::cout << "\nFaces detected " << faces.size();
    std::cout << "\nFace 1 " << faces[0];
    std::cout << "\nFace 1.x " << faces[0].x;
      std::cout << "\nFace 1.y " << faces[0].y;

      std::cout << "\nFace 2 " << faces[1];
      std::cout << "\nFace 2.x " << faces[1].x;
        std::cout << "\nFace 2.y " << faces[1].y;
  //Once we have the faces, iterate through them

  printf("\nA data are: width %d, height %d, total pixels %d\n", a.width, a.height, (a.width * a.height) );
    int len = a.width * a.height * 4;
int j = 0;
int k = 0;

  for(int i =0; i<len; i++)
{
    pixels[i] = a.pixels[i];
  }


int final_size = (faces[0].width < faces[1].width ? faces[0].width : faces[1].width);
uint8* face1 = (uint8*)malloc(final_size * final_size*4);
uint8* face2 = (uint8*)malloc(final_size * final_size*4);
j= 0;

printf("Final size %d\n", final_size * final_size );


  for(int x = faces[0].y; x < faces[0].y + final_size; x++){
      for(int y = faces[0].x*4 ; y < (faces[0].x + final_size)*4 ; y++){

       face1[j] = pixels[(a.width * 4 * x) + y];
       pixels[(a.width * 4 * x) + y] = 0; //ESto va de 4 en
      j++;

    }
  }

j = 0;

for(int x = faces[1].y; x < faces[1].y + final_size; x++){
  for(int y = faces[1].x*4 ; y < (faces[1].x + final_size)*4 ; y++){

     face2[j] = pixels[(a.width * 4 * x) + y];
     pixels[(a.width * 4 * x) + y] = 0; //ESto va de 4 en
    j++;

  }
}
 j = 0;
 for(int x = faces[0].y; x < faces[0].y + final_size; x++){
   for(int y = faces[0].x*4 ; y < (faces[0].x + final_size)*4 ; y++){

      pixels[(a.width * 4 * x) + y]=face2[j];

     j++;

   }
 }

  j = 0;
  for(int x = faces[1].y; x < faces[1].y + final_size; x++){
    for(int y = faces[1].x*4 ; y < (faces[1].x + final_size)*4 ; y++){

       pixels[(a.width * 4 * x) + y]=face1[j];

      j++;

    }
  }
}
  //Hace sto para que avance de 4 en 4 y meter todos los valores en un array para comparar lo con las muestras caucasicas que hemos cogido antes --> (255,224, 189) (255,205,148) (234, 192, 134) (255,173, 96) (255, 227 159)



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











int main()
{
    Image a;
    Image b;
    Mat original;
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
                original = imread(fnames[0].c_str());
                texture_image(&a);
              }

            if(ImGui::Button("Reset")){
                reset_image(&a);
                reset_image(&b);
                free_image(&a);
                free(a.pixels);
                free_image(&b);
                free(b.pixels);
            }
        }

           ImGui::EndGroup();
           ImGui::SameLine();

            ImGui::BeginGroup();
           {


                if(a.texture_loaded && !b.texture_loaded){
                if(ImGui::Button("Swap Faces")){
                  b.width = a.width;
                  b.height = a.height;
                  b.pixels = (uint8*)malloc(b.width * b.height * RGBA);
                  swap_faces(a, b.pixels, original);
                  texture_image(&b);
                  bits_name = "swap";

                }
              }
                if(b.texture_loaded){
                  ImGui::Image((void*)b.texture, resize(b.width, b.height), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,255,0,255));

                                  if (ImGui::Button("Save")) { //When clicked it saves the picture as bit_des.png
                                    char sname[1000];
                                    bits_name.append("_face_swap.png");
                                    sprintf(sname, bits_name.c_str());
                                    stbi_write_png(sname, b.width, b.height, 4, b.pixels, 0);
                                  }
                }



                }



            ImGui::EndGroup();
           //Create Image when process is clicked

            ImGui::SameLine();




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
