Durden - The large image tiler
==============================

What is Durden?
---------------
Durden is a command line program that will take a large image, and create the 
tiles necessary for the image to be viewed using various client software such 
as the Bigimage Google map image viewer.

Durden creates a number of directories (depending on the size of the image). 
These are numbered from 0 up. These folders represent the different zoom levels 
at which an image can be viewed. Zoom level 0 contains one image, 1 contains 4, 
2 contains 16 and so on (2^(2*zoomlevel)).

Install/Compile
---------------
Compilation is pretty simple. Firstly, you must ensure you have the ImageMagick 
(or GraphicsMagick if you prefer, although I have not yet tested this) 
development libraries installed. On a Debian based machine, this is as simple 
as running:

    apt-get install libmagick++-dev

Next simply run the compile script included. This should create the "durden" 
executable. If you have any issues, please post them to me, and I will attempt 
to help.

    ./compile

Assuming the binary file has been created, you should now copy it to somewhere 
in your path, e.g. ~/bin or /usr/local/bin.

    sudo cp durden /usr/local/bin

Usage
-----
Durden accepts two or three command line arguments. The first is the filename 
of an image, the second is a folder to output the tiles to (the folder will be 
created if it does not already exist), and the final optional argument is a 
tile size. The default til e size is 256x256 pixels - as supported by the 
Google Maps API.

The example below will read in the image "image.tiff" and create a folder 
"output" into which a number of sub-folders will be created. The number of 
sub-folders that are created will depend on the size of image.tiff.

    durden image.tiff output

Support
-------
Support requests should be posted to the [Big Image issues 
queue](http://drupal.org/project/issues/bigimage?categories=All), ensuring you 
specify "Durden" as the component. Unfortunately you will need a Drupal.org 
account before you can post a suppo rt request/bug report.

Durden?
-------
The name Durden is a reference to the character Tyler Durden in the book/film 
"Fight Club". This software is an image tiler and is also "...free in all the 
ways that you are not."
