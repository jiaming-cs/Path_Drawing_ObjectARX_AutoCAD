# Path_Drawing_ObjectARX_AutoCAD
This repository contains the source code of the automatic railway path drawing applcaition implemented by ObjectARX + MFC. You can input data from `.csv` file or MySQL database. The application will automatically draw the path in AutoCAD. By moving the grip point, you can adjust the path manually. Finally you can save the elements of a horizontal curve in .csv file including LONG CHORD, EXTERNAL DISTANCE, MIDDLE ORDINATE etc.

## TODO
* Add set environment in README
* Change app language to English
* Add English Comments in source code

## Usage
1. Clone project to local
```
git clone https://github.com/jiamingli9674/Path_Drawing_ObjectARX_AutoCAD.git
```

2. Open AutoCAD, type command `appload`, then press enter.

3. Load `.arx` file in path: `/Path_Drawing_ObjectARX_AutoCAD/DrawPathMFC/x64/Debug/DrawPathMFC.arx`

4. Type command `MYCOMMANDLOCAL` to run the application
![GUI](/img/GUI.png)

## Example

Read data from .csv file<br>
1. Click import point of intersection
![Import POI](/img/import_poi.png)
2. Slect from file (can be .txt or .csv)
![From File](/img/from_file.png)
3. Open the test data (`test_data.txt`) in the repo
![Open Test Data](/img/open_test_data.png)
4. If the format of the data is correct, the information is supposed to be imported into the applicaiton, click draw entity to draw the path in AutoCAD
![Draw Entity](/img/draw_entity.png)
5. Done
![Overview](/img/overview.png)
![Details](/img/details.png)



