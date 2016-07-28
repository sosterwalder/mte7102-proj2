#include "common.hpp"
// #include "source.hpp"
// #include "sink.hpp"
// #include "graph.hpp"
#include "cameragraphnode.hpp"


NAMESPACE_BEGIN(QCE);

std::string CameraGraphNode::calculateOutput()
{
    spdlog::get("qde")->debug("Node '{}': Calculating output", mId);
}

NAMESPACE_END(QCE);
