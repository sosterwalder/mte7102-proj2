#include "main.hpp"

void DirectPopup::refreshRelativePlacement() {
    Popup::refreshRelativePlacement();
    mPos = mAnchorPos - Vector2i(0, mAnchorHeight);
}
