#include "common.hpp"
#include "directpopup.hpp"


NAMESPACE_BEGIN(QCE);

void DirectPopup::refreshRelativePlacement() {
    Popup::refreshRelativePlacement();
    mPos = mAnchorPos - Eigen::Vector2i(0, mAnchorHeight);
}

bool DirectPopup::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)
{
    spdlog::get("qde")->debug(
        "DirectPopup '{}': Received click at ({},{})",
        mTitle, p.x(), p.y()
    );
    return Popup::mouseButtonEvent(p, button, down, modifiers);
}

NAMESPACE_END(QCE);