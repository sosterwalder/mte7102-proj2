#include <nanogui/widget.h>
#include "common.hpp"
#include "connector.hpp"
#include "graph.hpp"
#include "sink.hpp"


NAMESPACE_BEGIN(QCE);

Sink::Sink(nanogui::Widget *parent, Graph *parentGraph, const std::string &label) :
    Connector(parent, parentGraph, label)
{
}

Eigen::Vector2i Sink::relativePosition()
{
    return Eigen::Vector2i(
        5,
        mParent->size().y() * 0.5f
    );
}

NAMESPACE_END(QCE);