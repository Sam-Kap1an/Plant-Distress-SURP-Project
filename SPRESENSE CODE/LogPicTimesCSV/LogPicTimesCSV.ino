#include <SDHCI.h>
#include <stdio.h>  /* for sprintf */

#include <Camera.h>

#include "src/DonutStudioMillisTime.h"

#define BAUDRATE                (115200)
#define TOTAL_PICTURE_COUNT     (10)

SDClass  theSD;
int take_picture_count = 0;

MillisTime clkTime = MillisTime(); 

unsigned long previous;
int dataIndex = 0;

/**
 * Print error message
 */

void printError(enum CamErr err)
{
  Serial.print("Error: ");
  switch (err)
    {
      case CAM_ERR_NO_DEVICE:
        Serial.println("No Device");
        break;
      case CAM_ERR_ILLEGAL_DEVERR:
        Serial.println("Illegal device error");
        break;
      case CAM_ERR_ALREADY_INITIALIZED:
        Serial.println("Already initialized");
        break;
      case CAM_ERR_NOT_INITIALIZED:
        Serial.println("Not initialized");
        break;
      case CAM_ERR_NOT_STILL_INITIALIZED:
        Serial.println("Still picture not initialized");
        break;
      case CAM_ERR_CANT_CREATE_THREAD:
        Serial.println("Failed to create thread");
        break;
      case CAM_ERR_INVALID_PARAM:
        Serial.println("Invalid parameter");
        break;
      case CAM_ERR_NO_MEMORY:
        Serial.println("No memory");
        break;
      case CAM_ERR_USR_INUSED:
        Serial.println("Buffer already in use");
        break;
      case CAM_ERR_NOT_PERMITTED:
        Serial.println("Operation not permitted");
        break;
      default:
        break;
    }
}

/**
 * Callback from Camera library when video frame is captured.
 */

void CamCB(CamImage img)
{

  /* Check the img instance is available or not. */

  if (img.isAvailable())
    {

      /* Convert image data format to RGB565 to save space*/
      img.convertPixFormat(CAM_IMAGE_PIX_FMT_RGB565);

      /* You can use image data directly by using getImgSize() and getImgBuff().
       * for displaying image to a display, etc. */

      Serial.print("Image data size = ");
      Serial.print(img.getImgSize(), DEC);
      Serial.print(" bytes");
      Serial.print(" , ");

      Serial.print("buff addr = ");
      Serial.print((unsigned long)img.getImgBuff(), HEX);
      Serial.println("");
    }
  else
    {
      Serial.println("Failed to get video stream image");
    }
}

/**
  * Get time from milliseconds since reboot
  */
String GetTime() 
{
  if (millis() - previous >= 500) 
  {
    int absHrs = clkTime.getAbsoluteHours();
    int absMin = clkTime.getAbsoluteMinutes();
    int absSec = clkTime.getAbsoluteSeconds();

    String hrs = absHrs < 10 ? "0" + String(absHrs) : String(absHrs);
    String min = absMin < 10 ? "0" + String(absMin) : String(absMin);
    String sec = absSec < 10 ? "0" + String(absSec) : String(absSec);

    String time = hrs;
    time += ":";
    time += min;
    time += ":";
    time += sec;
    Serial.print(time);
    return(time);
    previous = millis();
  }
}

/**
  * Create an Image File with given file name
  */
String CreateImageFile(int take_picture_count) {

  CamImage img = theCamera.takePicture();

  /* Check availability of the img instance. Img is not available if errors occur. */
  if (img.isAvailable())
  {
    /* Create file name */
    char filename[16] = {0};
    sprintf(filename, "PICT%03d.JPG", take_picture_count);

    Serial.print("Save taken picture as ");
    Serial.print(filename);
    Serial.println("");

    /* Remove the old file with the same file name as new created file, and create new file. */

    theSD.remove(filename);
    File myFile = theSD.open(filename, FILE_WRITE);
    myFile.write(img.getImgBuff(), img.getImgSize());
    myFile.close();
    return filename;
  }
  else
  {
    /* If picture exceeds memory size
     *
     * Allocate larger memory size
     * - Decrease jpgbufsize_divisor specified by setStillPictureImageFormat()
     * - Increase the Memory size from Arduino IDE tools Menu
     * Decrease picture size
     * - Decrease the JPEG quality by setJPEGQuality()
     */

    Serial.println("Failed to take picture");
    return "-1";
  }
}

/**
  *
  */
void AppendDataFile(String imgFileName) {
  // make a string for assembling the data to log:
  String dataString = "";

  // read audio and append to the string:
  dataString += dataIndex;
  dataString += " , ";
  dataString += GetTime();
  dataString += " , ";
  //dataString += Audio File;
  dataString += " , ";
  dataString += imgFileName;
  dataString += ",";
  /* ... */

  // open the file - only one file can be open at a time, so must close
  File dataFile = theSD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    dataIndex += 1;
    Serial.println("Data Saved to Data Log.");
  }
  else {
    Serial.println("error opening datalog.csv");
  }
}

void setup()
{
  CamErr err;

  /* Open serial communications and wait for port to open */

  Serial.begin(BAUDRATE);
  while (!Serial)
    {
      ; /* wait for serial port to connect. Needed for native USB port only */
    }

  /* Initialize SD */
  while (!theSD.begin()) 
    {
      /* wait until SD card is mounted. */
      Serial.println("Insert SD card.");
    }
  /* Initialize Data File */
  
  theSD.remove("datalog.csv");
  File dataFile = theSD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("Index, Time Stamp, Audio File, Image File,");
    dataFile.close();

    Serial.println("Data Log Initialized.");
  }
  else {
    Serial.println("error initializing datalog.txt");
  }

  /* begin() without parameters means that
   * number of buffers = 1, 30FPS, QVGA, YUV 4:2:2 format */

  Serial.println("Prepare camera");
  err = theCamera.begin();
  if (err != CAM_ERR_SUCCESS)
    {
      printError(err);
    }

  /* Start video stream.
   * If received video stream data from camera device,
   *  camera library call CamCB.
   */

  Serial.println("Start streaming");
  err = theCamera.startStreaming(true, CamCB);
  if (err != CAM_ERR_SUCCESS)
    {
      printError(err);
    }

  /* Auto white balance configuration */

  Serial.println("Set Auto white balance parameter");
  err = theCamera.setAutoWhiteBalanceMode(CAM_WHITE_BALANCE_DAYLIGHT);
  if (err != CAM_ERR_SUCCESS)
    {
      printError(err);
    }
 
  /* Set parameters about still picture.
   * In the following case, QUADVGA and JPEG.
   */

  Serial.println("Set still picture format");
  err = theCamera.setStillPictureImageFormat(
     CAM_IMGSIZE_QUADVGA_H,
     CAM_IMGSIZE_QUADVGA_V,
     CAM_IMAGE_PIX_FMT_JPG);
  if (err != CAM_ERR_SUCCESS)
    {
      printError(err);
    }
}

void loop()
{
  sleep(1); /* wait for one second to take still picture. */

  if (take_picture_count < TOTAL_PICTURE_COUNT)
    {
      String imageFileName = CreateImageFile(take_picture_count);
      if (imageFileName != "-1") {
        AppendDataFile(imageFileName);
      }
    }
  else if (take_picture_count == TOTAL_PICTURE_COUNT)
    {
      Serial.println("End.");
      theCamera.end();
    }

  take_picture_count++;
}
