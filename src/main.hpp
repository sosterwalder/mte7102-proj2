#pragma once

// System imports
#include <iostream>
#include <fstream>

// Project imports
#include <spdlog/spdlog.h>
#include <src/pugixml.hpp>
#include <src/stringhelper.h>
#include <src/Jinja2CppLight.h>
#include <nanogui/screen.h>
#include <nanogui/theme.h>
#include <nanogui/window.h>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/popup.h>
#include <nanogui/textbox.h>
#include <nanogui/entypo.h>
#include <nanogui/glutil.h>
#include "util.hpp"
#include "property.hpp"
#include "glshaderobject.hpp"
#include "glshader.hpp"
#include "clickablelabel.hpp"
#include "directpopup.hpp"
#include "sink.hpp"
#include "inputsink.hpp"
#include "outputsink.hpp"
#include "graphnodelink.hpp"
#include "graphnode.hpp"
#include "outputgraphnode.hpp"
#include "genericgraphnode.hpp"
#include "graph.hpp"
#include "qce.hpp"

// Using defintions
using std::cout;
using std::cerr;
using std::endl;
