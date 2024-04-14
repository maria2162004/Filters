/*
File: CS112_A3_P2_S25_20231088_20231299_20231206.cpp

Authors:
Safia Mohamed Saied (S26)
Maria Atef Edward (S25)
Yehia Mohamed Adel (S25)

 E-mails:
safiam2006@gmail.com
mariaatef517@gmail.com
yahianassar2005@gmail.com

 ID's
20231088: Gray scale, Dark & light, Detecting edges, Merge and Purple filter.
20231299: Invert, Blur, Frame, Rotate and  Infrared filter.
20231206: black & white, flip, Resize, Crop and Sunlight filter.
*/


#include <iostream>
#include <algorithm>
#include "Image_Class.h"
#include <regex>
#include <array>
#include <limits>
#include "vector"


using namespace std;


bool isValidInput(const string& input) {
    regex rx("([a-zA-Z0-9_-]+)\\s*([a-zA-Z0-9_-]+)\\.(\\w+)$");
    //smatch match;
    return regex_match(input, rx); // Use regex_match with match object

}


string ValidName() {
    string filename;
    cout << "Pls enter image name:";
    smatch matches;

    while (true) {
        cin >> filename;
        // See if the input follow the regex format or not
        if (!isValidInput(filename)) {
            cout << "this is an invalid input format!" << endl;
            continue;
        }
        regex_match(filename, matches, regex("([a-zA-Z0-9_-]+)\\s*([a-zA-Z0-9_-]+)\\.(\\w+)$"));


        if (matches[3] != "jpg" && matches[3] != "png" && matches[3] != "bmp" && matches[3] != "tga") {
            cout << "this is not a valid extension" << endl;
            continue;
        }
        break;
    }
    return filename;
}



void Save(Image& image) {
    cout << "Pls enter image name to store new image\n";
    cout << "and specify extension .jpg, .bmp, .png, .tga: ";
    string newFilename = ValidName();
    image.saveImage(newFilename);
    system(newFilename.c_str());
}

void Rotate(Image& image) {
    cout << "Choose rotation angle:\n1) 90 degrees\n2) 180 degrees\n3) 270 degrees\n";
    int num;
    cin >> num;
    int height = image.height;
    int width = image.width;
    int channels = image.channels;

    Image RotatedImage;

    // Using if statements to determine rotation angle
    if (num == 1) { // 90 degrees clockwise
        RotatedImage = Image(height, width);
        for (int i = 0; i < image.width; i++) {
            for (int j = 0; j < image.height; j++) {
                for (int k = 0; k < 3; k++) {
                    RotatedImage(height - j - 1, i, k) = image(i, j, k);
                }
            }
        }
    }

    else if (num == 2) { // 180 degrees
        RotatedImage = Image(width, height);
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                for (int k = 0; k < 3; ++k) {
                    RotatedImage(width - i - 1, height - j - 1, k) = image(i, j, k);
                }
            }
        }

    }

    else if (num == 3) { // 270 degrees clockwise
        RotatedImage = Image(height, width);
        for (int i = 0; i < image.width; i++) {
            for (int j = 0; j < image.height; j++) {
                for (int k = 0; k < 3; k++) {
                    RotatedImage(j, i, k) = image(i, j, k);
                }
            }
        }
    }

    else {
        cout << "Invalid option!" << endl;
        return;
    }

    image = RotatedImage;
}



// Function to get frame thickness
int getFrameThickness() {
    int thickness;
    cout << "Select frame thickness (enter an integer value between 1 and 100): ";
    cin >> thickness;
    while (cin.fail() || thickness < 1 || thickness > 100) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter an integer value between 1 and 100: ";
        cin >> thickness;
    }
    return thickness;
}

// Function to choose frame type
string getFrameType() {
    string frameType;
    cout << "Choose frame type (normal, fancy): ";
    cin >> frameType;
    while (frameType != "normal" && frameType != "fancy") {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid frame type. Please enter 'normal' or 'fancy': ";
        cin >> frameType;
    }
    return frameType;
}

// Function to choose the frame color
string getFrameColor() {
    string frameColor;
    cout << "Enter the frame color (red, green, blue, purple, white, black): ";
    cin >> frameColor;
    while (frameColor != "red" && frameColor != "green" && frameColor != "blue" &&
        frameColor != "purple" && frameColor != "white" && frameColor != "black") {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid color. Please enter a valid color: ";
        cin >> frameColor;
    }
    return frameColor;
}

// Function to get RGB values for a color
array<int, 3> getRGBValues(const string& color) {
    if (color == "red") {
        return { 255, 0, 0 };
    }
    else if (color == "green") {
        return { 0, 255, 0 };
    }
    else if (color == "blue") {
        return { 0, 0, 255 };
    }
    else if (color == "purple") {
        return { 160, 32, 240 };
    }
    else if (color == "white") {
        return { 255, 255, 255 };
    }
    else if (color == "black") {
        return { 0, 0, 0 };
    }
    else {
        cerr << "Invalid color." << endl;
        exit(-1);
    }
}

// Function for fancy frame
Image FancyFrame(Image& image, int thickness, const array<int, 3>& frameColor) {
    int width = image.width;
    int height = image.height;

    Image modifiedImage(width + 2 * thickness, height + 2 * thickness);

    // Draw the top border
    for (int x = 0; x < modifiedImage.width; ++x) {
        for (int t = 0; t < thickness; ++t) {
            if (t % 3 == 0) { // Draw a space between double lines
                modifiedImage.setPixel(x, t, 0, frameColor[0]);
                modifiedImage.setPixel(x, t, 1, frameColor[1]);
                modifiedImage.setPixel(x, t, 2, frameColor[2]);
            }
        }
    }

    // Draw the bottom border
    for (int x = 0; x < modifiedImage.width; ++x) {
        for (int t = 0; t < thickness; ++t) {
            if (t % 3 == 0) {
                modifiedImage.setPixel(x, modifiedImage.height - 1 - t, 0, frameColor[0]);
                modifiedImage.setPixel(x, modifiedImage.height - 1 - t, 1, frameColor[1]);
                modifiedImage.setPixel(x, modifiedImage.height - 1 - t, 2, frameColor[2]);
            }
        }
    }

    // Draw the left border
    for (int y = thickness; y < modifiedImage.height - thickness; ++y) {
        for (int t = 0; t < thickness; ++t) {
            if (t % 3 == 0) {
                modifiedImage.setPixel(t, y, 0, frameColor[0]);
                modifiedImage.setPixel(t, y, 1, frameColor[1]);
                modifiedImage.setPixel(t, y, 2, frameColor[2]);
            }
        }
    }

    // Draw the right border
    for (int y = thickness; y < modifiedImage.height - thickness; ++y) {
        for (int t = 0; t < thickness; ++t) {
            if (t % 3 == 0) {
                modifiedImage.setPixel(modifiedImage.width - 1 - t, y, 0, frameColor[0]);
                modifiedImage.setPixel(modifiedImage.width - 1 - t, y, 1, frameColor[1]);
                modifiedImage.setPixel(modifiedImage.width - 1 - t, y, 2, frameColor[2]);
            }
        }
    }
    // Define pink color RGB values
    const array<int, 3> pinkColor = { 255, 105, 180 };

    // Draw top-left corner box  (pink)
    for (int x = -thickness; x < 0; ++x) {
        for (int y = -thickness; y < 0; ++y) {
            if ((x + y) % 3 == 0) {
                modifiedImage.setPixel(x + thickness, y + thickness, 0, pinkColor[0]);
                modifiedImage.setPixel(x + thickness, y + thickness, 1, pinkColor[1]);
                modifiedImage.setPixel(x + thickness, y + thickness, 2, pinkColor[2]);
            }
        }
    }

    // Draw top-right corner box (pink)
    for (int x = width; x < width + thickness; ++x) {
        for (int y = -thickness; y < 0; ++y) {
            if ((x - width + y) % 3 == 0) {
                modifiedImage.setPixel(x + thickness, y + thickness, 0, pinkColor[0]);
                modifiedImage.setPixel(x + thickness, y + thickness, 1, pinkColor[1]);
                modifiedImage.setPixel(x + thickness, y + thickness, 2, pinkColor[2]);
            }
        }
    }

    // Draw bottom-left corner box (pink)
    for (int x = -thickness; x < 0; ++x) {
        for (int y = height; y < height + thickness; ++y) {
            if ((y - height + x) % 3 == 0) {
                modifiedImage.setPixel(x + thickness, y + thickness, 0, pinkColor[0]);
                modifiedImage.setPixel(x + thickness, y + thickness, 1, pinkColor[1]);
                modifiedImage.setPixel(x + thickness, y + thickness, 2, pinkColor[2]);
            }
        }
    }

    // Draw bottom-right corner box (pink)
    for (int x = width; x < width + thickness; ++x) {
        for (int y = height; y < height + thickness; ++y) {
            if ((x - width + y - height) % 3 == 0) {
                modifiedImage.setPixel(x + thickness, y + thickness, 0, pinkColor[0]);
                modifiedImage.setPixel(x + thickness, y + thickness, 1, pinkColor[1]);
                modifiedImage.setPixel(x + thickness, y + thickness, 2, pinkColor[2]);
            }
        }
    }


    // Copy the input image onto the modified image, leaving the frame area blank
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) {
                modifiedImage.setPixel(x + thickness, y + thickness, c, image.getPixel(x, y, c));
            }
        }
    }

    return modifiedImage;
}

Image NormalFrame(Image& image, int thickness, const array<int, 3>& frameColor) {
    int width = image.width;
    int height = image.height;

    // Create a new image with the required dimensions for the frame
    Image modifiedImage(width + 2 * thickness, height + 2 * thickness);

    // Draw the top border
    for (int x = 0; x < modifiedImage.width; ++x) {
        for (int t = 0; t < thickness; ++t) {
            modifiedImage.setPixel(x, t, 0, frameColor[0]);
            modifiedImage.setPixel(x, t, 1, frameColor[1]);
            modifiedImage.setPixel(x, t, 2, frameColor[2]);
        }
    }


    // Draw the bottom border
    for (int x = 0; x < modifiedImage.width; ++x) {
        for (int t = 0; t < thickness; ++t) {
            modifiedImage.setPixel(x, modifiedImage.height - 1 - t, 0, frameColor[0]);
            modifiedImage.setPixel(x, modifiedImage.height - 1 - t, 1, frameColor[1]);
            modifiedImage.setPixel(x, modifiedImage.height - 1 - t, 2, frameColor[2]);
        }
    }


    // Draw the left border
    for (int y = 0; y < modifiedImage.height; ++y) {
        for (int t = 0; t < thickness; ++t) {
            modifiedImage.setPixel(t, y, 0, frameColor[0]);
            modifiedImage.setPixel(t, y, 1, frameColor[1]);
            modifiedImage.setPixel(t, y, 2, frameColor[2]);
        }
    }


    // Draw the right border
    for (int y = 0; y < modifiedImage.height; ++y) {
        for (int t = 0; t < thickness; ++t) {
            modifiedImage.setPixel(modifiedImage.width - 1 - t, y, 0, frameColor[0]);
            modifiedImage.setPixel(modifiedImage.width - 1 - t, y, 1, frameColor[1]);
            modifiedImage.setPixel(modifiedImage.width - 1 - t, y, 2, frameColor[2]);
        }
    }


    // Copy the input image onto the modified image, leaving the frame area blank
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) {
                modifiedImage.setPixel(x + thickness, y + thickness, c, image.getPixel(x, y, c));
            }
        }
    }

    return modifiedImage;
}



// Function convert image to black and white
void BlackWhite(Image& image) {

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned  int avg = 0;

            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k);
            }

            avg /= 3;
            if (avg < 126) {
                avg = 0;
            }
            else {
                avg = 255;
            }
            image(i, j, 0) = avg;
            image(i, j, 1) = avg;
            image(i, j, 2) = avg;

        }
    }
}


void Flip(Image& image) {
    cout << "do you want to flip\n1)horizontally\n2)vertically";
    int flip;
    cin >> flip;
    // Create a temporary image to hold the flipped pixels
    Image flippedImage(image.width, image.height);

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            if (flip == 1) {
                for (int k = 0; k < 3; ++k) {
                    flippedImage(i, j, k) = image(image.width - 1 - i, j, k);
                }
            }
            else {
                for (int k = 0; k < 3; ++k) {
                    flippedImage(i, j, k) = image(i, image.height - 1 - j, k);
                }
            }
        }
    }
    Save(flippedImage);

}



void Resize(Image& image) {
    int newwidth, newheight;
    cout << "Enter the dimention of the resized image:" << endl;
    cin >> newwidth >> newheight;

    double n, m;

    m = static_cast<double>(image.width) / newwidth;
    n = static_cast<double>(image.height) / newheight;

    Image image2(newwidth, newheight);
    for (int i = 0; i < image2.width; i++) {
        for (int j = 0; j < image2.height; j++) {
            for (int k = 0; k < image2.channels; k++) {
                image2(i, j, k) = image(round(i * m), round(j * n), k);
            }
        }
    }
    Save(image2);

}


void invert(Image& image) {

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = 255 - image(i, j, k);
            }
        }
    }
}


void GreyScale(Image& image) {

    for (int i = 0; i < image.width; ++i) {

        for (int j = 0; j < image.height; ++j) {
            pixel avg = 0;

            for (int k = 0; k < 3; ++k) {


                avg += image(i, j, k);
            }
            avg = avg / 3;
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = avg;
            }
        }

    }
}


void DarkLight(Image& image) {
    cout << "1)Darken\n2)lighen\n";
    int choice;
    cin >> choice;

    double brightness;

    if (choice == 1) {
        brightness = 0.5;  // value of darkness 50%
    }
    else {
        brightness = 1.5;    // value of brightness 50%
    }

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {

            for (int k = 0; k < 3; ++k) {

                int new_value = static_cast<int>(image(i, j, k) * brightness);
                // Make sure the value doesn't exceed 225
                image(i, j, k) = static_cast<unsigned char>(min(max(new_value, 0), 255));
            }
        }
    }

}

void Purple(Image& image) {

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            pixel red = image(i, j, 0);
            pixel green = image(i, j, 1);
            pixel blue = image(i, j, 2);

            // Set the channels to create a subtle purple tone
            image(i, j, 0) = red;  // Red
            image(i, j, 1) = green * 0.6;  // Green (unchanged)
            image(i, j, 2) = blue;  // Blue
        }
    }

}

//Function to detect edges
void Edges(Image& image) {

    BlackWhite(image);
    int width = image.width;
    int height = image.height;
    for (int i = 0; i < width - 1; ++i) {
        for (int j = 0; j < height - 1; ++j) {

            if (image(i, j, 0) != image(i, j + 1, 0) || image(i + 1, j, 0) != image(i, j + 1, 0)) {

                image(i, j, 0) = 0;
                image(i, j, 1) = 0;
                image(i, j, 2) = 0;
            }

            else {
                image(i, j, 0) = 255;
                image(i, j, 1) = 255;
                image(i, j, 2) = 255;
            }
        }
    }
}


// Function to merge two images
void Merge(Image& image) {

    string filename1 = ValidName();
    Image image2(filename1);

    cout << "Do you want to merge to bigger or smaller?\n1)Smaller\n2)Bigger\n";
    char choice;
    cin >> choice;

    if (choice == '1') {
        Image mergedImage(min(image.width, image2.width), min(image.height, image2.height));

        for (int i = 1; i < mergedImage.width; ++i) {
            for (int j = 1; j < mergedImage.height; ++j) {
                for (int k = 0; k < 3; ++k) {
                    mergedImage(i, j, k) = (image(i, j, k) + image2(i, j, k)) / 2;
                }
            }
        }

        Save(mergedImage);

    }

    else {

        // If merging to a larger image, resize both images to the larger dimensions
        int maxWidth = max(image.width, image2.width);
        int maxHeight = max(image.height, image2.height);

        double n, m;
        Image Resized1(maxWidth, maxHeight);
        Image Resized2(maxWidth, maxHeight);


        if (image.width > image2.width && image.height > image2.height) {
            m = static_cast<double>(image2.width) / maxWidth;
            n = static_cast<double>(image2.height) / maxHeight;

            for (int i = 0; i < Resized2.width; i++) {
                for (int j = 0; j < Resized2.height; j++) {
                    for (int k = 0; k < Resized2.channels; k++) {
                        Resized2(i, j, k) = image2(round(i * m), round(j * n), k);
                    }
                }
            }
            Resized1 = image;

        }
        else {
            m = static_cast<double>(image.width) / maxWidth;
            n = static_cast<double>(image.height) / maxHeight;

            for (int i = 0; i < Resized1.width; i++) {
                for (int j = 0; j < Resized1.height; j++) {
                    for (int k = 0; k < Resized1.channels; k++) {
                        Resized1(i, j, k) = image(round(i * m), round(j * n), k);
                    }
                }
            }
            Resized2 = image2;

        }

        // Now that both images have the same dimensions, perform the merging operation
        Image mergedImage(maxWidth, maxHeight);
        for (int i = 0; i < maxWidth; ++i) {
            for (int j = 0; j < maxHeight; ++j) {
                for (int k = 0; k < 3; ++k) {
                    mergedImage(i, j, k) = (Resized1(i, j, k) + Resized2(i, j, k)) / 2;
                }
            }
        }

        Save(mergedImage);

    }

}



void Infrared(Image& image1) {
    int width = image1.width;
    int height = image1.height;
    Image infrared(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            infrared(i, j, 0) = 255;
            infrared(i, j, 1) = 255 - image1(i, j, 1);
            infrared(i, j, 2) = 255 - image1(i, j, 2);
        }
    }
    // Copy the new image to the original image
    swap(image1.imageData, infrared.imageData);
}


// Function to calculate prefix sum for a given color channel
vector<vector<int>> calculatePrefixSum(const Image& image, int colorChannel) {
    int width = image.width;
    int height = image.height;
    vector<vector<int>> prefixSum(width, vector<int>(height, 0));

    // Calculate the prefix sum
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int pixelValue = image.getPixel(i, j, colorChannel);
            prefixSum[i][j] = pixelValue;

            if (i > 0)
                prefixSum[i][j] += prefixSum[i - 1][j];
            if (j > 0)
                prefixSum[i][j] += prefixSum[i][j - 1];
            if (i > 0 && j > 0)
                prefixSum[i][j] -= prefixSum[i - 1][j - 1];
        }
    }

    return prefixSum;
}


// Function to blur the image using prefix sum
Image blurImage(const Image& image, int blurRadius) {
    int width = image.width;
    int height = image.height;
    Image blurredImage(width, height);

    // Blur each color channel separately
    for (int colorChannel = 0; colorChannel < 3; ++colorChannel) {

        vector<vector<int>> prefixSum = calculatePrefixSum(image, colorChannel);

        // Calculate the blur for each pixel in the current color channel
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                // Calculate the bounds of the blur
                int left_bound = max(0, i - blurRadius);
                int right_bound = min(width - 1, i + blurRadius);
                int top_bound = max(0, j - blurRadius);
                int bottom_bound = min(height - 1, j + blurRadius);

                // Calculate the sum of pixel values within the blur bounds using prefix sum
                int sum = prefixSum[right_bound][bottom_bound];
                if (left_bound > 0)
                    sum -= prefixSum[left_bound - 1][bottom_bound];
                if (top_bound > 0)
                    sum -= prefixSum[right_bound][top_bound - 1];
                if (left_bound > 0 && top_bound > 0)
                    sum += prefixSum[left_bound - 1][top_bound - 1];

                // Calculate the average pixel within the blur bounds
                int count = (right_bound - left_bound + 1) * (bottom_bound - top_bound + 1);
                int avg = sum / count;

                // assign avg to new image as we use the avg of pixel to get blurred image
                blurredImage.setPixel(i, j, colorChannel, static_cast<unsigned char>(avg));
            }
        }
    }

    return blurredImage;
}


// Function to increase sunlight in the image
void sunlight(Image& image) {
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            // Boost the red and green channels to increase yellow color
            for (int k = 0; k < 2; k++) {
                // Increase the red and green channels by adding a value
                // You can adjust the value to control the intensity of sunlight
                image(i, j, k) = min(255, image(i, j, k) + 50); // Adding 50 to each channel
            }
        }
    }
}

void Crop(Image& image) {

    int newwidth, newheight, startW, startH;
    cout << "Enter the dimensions of the cropped image: " << endl;
    cin >> newwidth >> newheight;
    cout << "Enter the starting points: " << endl;
    cin >> startW >> startH;

    Image image2(newwidth, newheight);

    for (int i = 0; i < newwidth; i++) {
        for (int j = 0; j < newheight; j++) {
            for (int k = 0; k < 3; k++) {
                // Adjust coordinates to account for starting points
                int original_x = i + startW;
                int original_y = j + startH;

                // Make sure the coordinates are within the original image bounds
                original_x = std::max(0, std::min(original_x, image.width - 1));
                original_y = std::max(0, std::min(original_y, image.height - 1));

                // Set pixel value in cropped image
                image2.setPixel(i, j, k, image.getPixel(original_x, original_y, k));
            }
        }
    }
    Save(image2);
}


int main() {
    while (true) {

        string again;
        cout << "Do you want to edit an image or exit (Y/N)?: ";
        cin >> again;
        for (char& i : again)
            i = toupper(i);

        while (again != "Y" && again != "N") {
            cout << "Invalid input. Please enter Y or N: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> again;
            for (char& i : again)
                i = toupper(i);
        }

        if (again == "Y") {

            string filename = ValidName();
            Image image(filename);
            cout << "1)Dark & light\n2)Invert\n3)Grey scale\n4)Flip\n5)Black & white\n6)Purple\n7)Detect edges\n8)Frame"
                "\n9)Infrared\n10)Rotate\n11)Resize\n12)Blurr\n13)Sunlight\n14)Merge\n15)Crop\n";
            string choice;
            cin >> choice;

            while (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" && choice != "6" &&
                choice != "7" && choice != "8" && choice != "9" && choice != "10" && choice != "11" &&
                choice != "12" && choice != "13" && choice != "14" && choice != "15") {
                cout << "Enter a valid choice please:";
                cin >> choice;
            }

            if (choice == "1") {
                DarkLight(image);
                Save(image);
            }
            else if (choice == "2") {
                invert(image);
                Save(image);
            }
            else if (choice == "3") {
                GreyScale(image);
                Save(image);
            }
            else if (choice == "4") {
                Flip(image);
            }
            else if (choice == "5") {
                BlackWhite(image);
                Save(image);
            }
            else if (choice == "6") {
                Purple(image);
                Save(image);
            }
            else if (choice == "7") {
                Edges(image);
                Save(image);
            }
            else if (choice == "8") {
                int frameThickness = getFrameThickness();
                string frameType = getFrameType();
                string color = getFrameColor();
                array<int, 3> frameColor = getRGBValues(color);
                Image modifiedImage;
                if (frameType == "normal") {
                    modifiedImage = NormalFrame(image, frameThickness, frameColor);
                    modifiedImage.saveImage("normal_frame.jpg");
                }
                else if (frameType == "fancy") {
                    modifiedImage = FancyFrame(image, frameThickness, frameColor);
                    modifiedImage.saveImage("fancy_frame.jpg");
                }
                (image = modifiedImage);
                Save(image);
            }
            else if (choice == "9") {
                Infrared(image);
                Save(image);
            }
            else if (choice == "10") {
                Rotate(image);
                Save(image);
            }
            else if (choice == "11") {

                Resize(image);


            }
            else if (choice == "12") {
                //allow the user to enter the blur level
                int blurLevel;
                while (true) {
                    cout << "Enter the blur level (1 for light, 2 for medium, 3 for extreme): ";
                    cin >> blurLevel;

                    if (blurLevel == 1 || blurLevel == 2 || blurLevel == 3) {
                        break;
                    }
                    else {
                        cout << "Invalid blur level, please enter either 1, 2, or 3: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }

                int blurRadius;
                if (blurLevel == 1) {
                    blurRadius = 8;
                }
                else if (blurLevel == 2) {
                    blurRadius = 30;
                }
                else if (blurLevel == 3) {
                    blurRadius = 57;
                }
                Image blurredImage = blurImage(image, blurRadius);
                (image = blurredImage);
                Save(image);

            }
            else if (choice == "13") {
                sunlight(image);
                Save(image);
            }
            else if (choice == "14") {
                Merge(image);
            }
            else if (choice == "15") {
                Crop(image);

            }
        }
        else {
            exit(0);
        }

}

}
