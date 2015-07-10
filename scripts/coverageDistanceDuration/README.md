# coverageDistanceDuration

This tool helps you calculate the distance, the duration and the coverage from your measurements in the teleworkbench.

For a precise calculation it is important, that your tracking data starts with the beginning of the operation and ends shortly after its completion.

## Build the program
There is a binary for x86_64 systems, but if you want to build the program on your own, use the files in the src folder and keep in mind that you will need the Qt framework (>= 5.0) either way.

## Run the program
To run the program be aware that you have the following files in your working directory:
* CAMERA_1_tracking_data.txt
* CAMERA_2_tracking_data.txt
* CAMERA_3_tracking_data.txt
* CAMERA_4_tracking_data.txt
* scenario_with_walls.png (can be found in the coverageDistanceDuration folder)

The program itself then can be executed by the following command:

`<PathToProgram>/vacCleanAnalyzer <TrackerID> <Diameter of Robot in cm>`

## The result

The program will print out results in the following way for every 5 minute marker:
```
########################
After 25 minutes:

Trackpoints: 34228
Coverage:    39.7 %
Distance:    70872.9 px
             236.243 m
Duration:    1500 sec
             00:25:00 (hh:mm:ss)
```

Furthermore a summary will be printed out and three types of coverage images are exported into your working directory:
* coverage.png
* scenarioAndCoverage_XXXm.png
* scenarioAndCoverage_final.png

## Comparison

To compare all of our results, please add your data into our spreadsheet:
[LINK TO SPREADSHEET](https://docs.google.com/spreadsheets/d/1Hql7qDrgEm0oKqIBd6pix0cMSshNFj__7QqPNpNRtXE/edit?usp=sharing)
