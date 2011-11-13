//============================================================================
// Name        : durden.cpp
// Author      : S.D.Rycroft
// Version     : 1
// Copyright   : GPLv3 Blah blah blah
// Description : Image tiler for Google maps.
//============================================================================
#include <Magick++.h>
#include <iostream>
#include <sys/stat.h>
using namespace std;
using namespace Magick;
bool FileExists(string FileName){
  FILE* fp = NULL;
  fp = fopen( FileName.c_str(), "rb" );
  if( fp != NULL ){
    fclose( fp );
    return true;
  }
  return false;
}
int main(int argc,char **argv)
{
  // CHeck we have arguments.
  if(argc <= 2){
    cout << "Usage: $ durden image_filename output_dir [tile_size]" << endl;
    exit(1);
  }
  // Variables
  Image image;
  Image image_crop;
  Image image_merge;
  char x_str[21];
  char y_str[21];
  char prev_x_str[21];
  char prev_y_str[21];
  char prev_x_plus_1_str[21];
  char prev_y_plus_1_str[21];
  char zoom_str[21];
  char prev_zoom_str[21];
  const char *directory;
  string pre_filename = argv[2];
  string post_filename = ".jpg";
  string directory_str;
  string filename;
  int image_x;
  int image_y;
  int current_tile_size;
  int tile_size;
  int zoom_levels = 0;
  // Set the tile_size from the 3rd variable if set.
  if(argc >= 4){
    sscanf(argv[3], "%u", &tile_size);
  } else {
    tile_size = 256;
  }
  current_tile_size = tile_size;
  // Construct the image object. Seperating image construction from the
  // the read operation ensures that a failure to read the image file
  // doesn't render the image object useless.
  try {
    // Get the dimensions of the original image.
    if(!FileExists(argv[1])){
      cout << "Image not found: " << argv[1] << endl;
      return 1;
    }
    image.read(argv[1]);
    image_x = (int)image.columns();
    image_y = (int)image.rows();
    // Create the output directory.
    directory = pre_filename.c_str();
    mkdir(directory, 0755);
    // Calculate the zoom levels, and create the directories for each one.
    int i = tile_size;
    while(i < (image_x * 2) || i < (image_y * 2)){
      sprintf(zoom_str, "%u", zoom_levels);
      directory_str = pre_filename + "/" + zoom_str;
      directory = directory_str.c_str();
      mkdir(directory, 0755);
      zoom_levels ++;
      i = i * 2;
    }
    // Start with the max zoom level - This zoom level is special, as it is
    // the only one that uses the original image, all other zoom levels stitch
    // together four images from the level below (and then resize).
    zoom_levels --;
    sprintf(zoom_str, "%u", zoom_levels);
    for(int x=0;x<image_x;x+=current_tile_size){
      sprintf(x_str, "%u", x/current_tile_size);
      for(int y=0;y<image_y;y+=current_tile_size){
        sprintf(y_str, "%u", y/current_tile_size);
        image_crop = image;
        image_crop.crop(Geometry(current_tile_size,current_tile_size,x,y));
        image_crop.write(pre_filename + "/" + zoom_str + "/" + x_str + "-" + y_str + post_filename);
      }
    }
    // Next we do all the other zoom levels.
    for(int i = zoom_levels - 1; i >=0; i--){
      current_tile_size = current_tile_size * 2;
      sprintf(zoom_str, "%u", i);
      sprintf(prev_zoom_str, "%u", i+1);
      for(int x=0;x<image_x;x+=current_tile_size){
        sprintf(x_str, "%u", x/current_tile_size);
        sprintf(prev_x_str, "%u", (x * 2)/current_tile_size);
        sprintf(prev_x_plus_1_str, "%u", ((x * 2)/current_tile_size)+1);
        for(int y=0;y<image_y;y+=current_tile_size){
          sprintf(y_str, "%u", y/current_tile_size);
          sprintf(prev_y_str, "%u", (y * 2)/current_tile_size);
          sprintf(prev_y_plus_1_str, "%u", ((y * 2)/current_tile_size)+1);
          if(FileExists(pre_filename + "/" + prev_zoom_str + "/" + prev_x_str + "-" + prev_y_str + post_filename)){
            image_crop.read(pre_filename + "/" + prev_zoom_str + "/" + prev_x_str + "-" + prev_y_str + post_filename);
            // Increase the image size to add the other thumbnails.
            image_crop.extent(Geometry(tile_size*2, tile_size*2, 0, 0));
            // Add NE.
            if(FileExists(pre_filename + "/" + prev_zoom_str + "/" + prev_x_plus_1_str + "-" + prev_y_str + post_filename)){
              image_merge.read(pre_filename + "/" + prev_zoom_str + "/" + prev_x_plus_1_str + "-" + prev_y_str + post_filename);
              image_crop.composite(image_merge, tile_size, 0);
            }
            // Add SE.
            if(FileExists(pre_filename + "/" + prev_zoom_str + "/" + prev_x_plus_1_str + "-" + prev_y_plus_1_str + post_filename)){
              image_merge.read(pre_filename + "/" + prev_zoom_str + "/" + prev_x_plus_1_str + "-" + prev_y_plus_1_str + post_filename);
              image_crop.composite(image_merge, tile_size, tile_size);
            }
            // Add SW.
            if(FileExists(pre_filename + "/" + prev_zoom_str + "/" + prev_x_str + "-" + prev_y_plus_1_str + post_filename)){
              image_merge.read(pre_filename + "/" + prev_zoom_str + "/" + prev_x_str + "-" + prev_y_plus_1_str + post_filename);
              image_crop.composite(image_merge, 0, tile_size);
            }
            // Resize
            image_crop.scale(Geometry(tile_size, tile_size, 0, 0));
            image_crop.write(pre_filename + "/" + zoom_str + "/" + x_str + "-" + y_str + post_filename);
          }
        }
      }
    }
  }
  catch( Exception &error_ ){
    cout << "Caught exception: " << error_.what() << endl;
    return 1;
  }
  return 0;
}
