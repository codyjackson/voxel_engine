Voxel Engine
============
A basic voxel rendering engine.


Project Setup
------------
###Additional Files:
You will need to download the [shared_unversioned.zip](https://dl.dropboxusercontent.com/u/6969315/shared_unversioned.zip) directories and extract it so that you get a file-structure like:

```sh
voxel_engine/
    lib/
        bin/
        include/
        symbols/
        
    src/
        browser/
        constants/
        rendering/
        spatial/
        utility/
        ...
        
    resources/
        locales/
        cef.pak
        ...
        
    ui/
        ...
```

I decided to keep the lib directory separate from the repo since it contains binary and symbol files which will quickly eat up the alloted 1GB space limit.

###Dependencies
1. [node.js](http://nodejs.org/)
2. [mimosa.js](http://mimosa.io/)  
    ```sh
    npm install -g mimosa
    ```
