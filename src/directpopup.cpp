#include "main.hpp"

void DirectPopup::refreshRelativePlacement() {
    Popup::refreshRelativePlacement();
    mPos = mAnchorPos - Vector2i(0, mAnchorHeight);
}

bool DirectPopup::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    spdlog::get("qde")->debug(
        "DirectPopup '{}': Received click at ({},{})",
        mTitle, p.x(), p.y()
    );
    return Popup::mouseButtonEvent(p, button, down, modifiers);
}
