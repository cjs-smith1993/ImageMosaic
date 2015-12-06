# ImageMosaic
Final group project for CS 484

Steps to get set up:
1. Install ToyViewer from the App Store so you can view PPMs
2. Install `gcc` (OSX uses  LLVM’s clang compiler, not gcc): `sudo brew install --with-all-languages --without-multilib gcc` (this will probably take about an hour and make your computer sound like it’s melting)
3. Install `imagemagick` to get access to the `convert` utility: `sudo brew install imagemagick` (this should finish in a couple minutes)
4. Clone the repo: `git clone git@github.com:cjs-smith1993/ImageMosaic.git`
5. Set up a couple directories: `cd ImageMosaic && mkdir bin output`
6. Compile the project: `make`
7. Run the project on the images, creating a 5x5 grid: `./bin/mosaic.o images_original/ 5 5`
8. Open the mosaic: `open output/mosaic.ppm`
9. Rejoice!