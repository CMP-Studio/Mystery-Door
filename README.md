# Mystery-Door
Code for the mystery door project, an interactive exhibit for the Natural History Museum. All code is written using [openFrameworks](http://openframeworks.cc/) in C++. 

## Description 

The Section of Mystery is a custom interactive experience for Carnegie Museum of Natural History (CMNH) that connects visitors to a diverse set of specimen from the collection. The interactive centers around one of the museum’s most notable and famous architectural features, a half-size wooden door in the Hall of Birds. The door now reads “Section of Mystery” in gold lettering across the front and constantly has mysterious animal noises emanating from within. Every time a museum visitor follows their curiosity and opens the door they are not disappointed by a lock or a maintenance closet. The space within illuminates and reveals a drifting hologram of a chirping or roaring specimen from the collection. The door hides dozens of animals, including a whale, various owls, and a Cape Buffalo, any one of which might emerge each time the door is opened.

![alt text](https://raw.githubusercontent.com/CMP-Studio/Mystery-Door/f55113dc089fd838d84c4b7ae7c28a61660e95c1/coverImage.jpg )

[Documentation video](https://vimeo.com/167481467) 

[Full Process write up](https://studio.carnegiemuseums.org/introducing-the-section-of-mystery-ef883d98b94f#.t13nojwoo)

## Targets

Only tested on Mac OS 10.11

## Dependecies

* openFrameworks version 8.4. [Here](http://openframeworks.cc/download/) are the openFrameworks install instructions. 
* ofxAssimpModelLoader
* ofxCameraSaveLoad
* ofxCsv
* ofxDmx
* ofxGui
* ofxOpenCv
* ofxXmlSettings

## Necessary Hardware
* DMX dimmer
* arduino reed switch attached to the door
* Pepper's Ghost Rig

## Intellectual Property
The animal sounds were sourced from the [Cornell Macaulay Library](http://macaulaylibrary.org/) and have been removed. I have left two samples of the kind of data it needs for each specimen. The rest of the animal models can be viewed and downloaded from [our sketchFab account](https://sketchfab.com/cmp_innovation_studio/models). 

