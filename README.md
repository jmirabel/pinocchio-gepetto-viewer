# pinocchio-gepetto-viewer

C++ client to gepetto-viewer for Pinocchio.

It depends on:
- pinocchio
- gepetto-viewer-corba

## Compilation

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install_prefix ..
make install
```

## Usage

```cpp
#include <pinocchio/gepetto/viewer.hh>

pinocchio::gepetto::Viewer viewer (model,
 &visualModel, // or NULL
 &collisionModel // or NULL
);
viewer.initViewer("pinocchio"); // window name
viewer.loadViewerModel("ur5"); // scene name

viewer.display(pin::neutral(model));
```