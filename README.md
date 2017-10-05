# Small code for using Embree renderer for beginners like me
## Embree
Embree renderer is one of the cutting-edge renderer built by Intel. It is a thin wrapper system aiming realistic rendering in Computer Graphics with Intel processor. In these days, physically based rendering is widely used in production-level movies. To make a physically based rendered image, "Ray-Tracing" technology is necessary. Hoever, even though the basic alogorighm of Ray-Tracing is not so different between several softwares* and it is not so complex, knowledge about the computer graphics is required to implement it. It makes a mind wall for beginners intereseted in Computer Graphics like me.

Embree provides their own API to hide too basic implementations to realize Ray-Tracing. It allow users to consentrate on making their own renderer. 

*) I think, the basic algorithm of Ray-Tracing is just throw rays and get pixel colors.
## Dependency
- Currently, this program is using **openCV** to view rendered images. If you do not want to use it, please delete/rename/replace the function named *writeImage*.
- Of course, this program uses **Embree renderer**, so that you should link a Embree library to this program.