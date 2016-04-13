#include "main.hpp"


std::map<std::string, GLShaderBuiltinType> GLShaderObject::BUILTIN_STR_TO_TYPE = {
    { "unknown", GLShaderBuiltinType::UNKNOWN},
    { "float",   GLShaderBuiltinType::FLOAT  },
    { "vec3",    GLShaderBuiltinType::VEC3   }
};
std::map<GLShaderBuiltinType, std::string> GLShaderObject::TYPE_TO_BUILTIN_STR = {
    { GLShaderBuiltinType::UNKNOWN, "unknown"},
    { GLShaderBuiltinType::FLOAT,   "float"  },
    { GLShaderBuiltinType::VEC3,    "vec3"   }
};
std::map<std::string, GLShaderType> GLShaderObject:: STR_TO_TYPE = {
    { "unknown",  GLShaderType::UNKNOWN },
    { "property", GLShaderType::PROPERTY},
    { "sink",     GLShaderType::SINK    }
};
std::map<GLShaderType, std::string> GLShaderObject:: TYPE_TO_STR = {
    { GLShaderType::UNKNOWN,  "unknown"  },
    { GLShaderType::PROPERTY, "property" },
    { GLShaderType::SINK,     "sink"     }
};
int GLShaderObject::mTimesUsed = 0;

GLShaderObject::GLShaderObject() :
    mId(mTimesUsed),
    mName("ShaderObject"),
    mReturnType(""),
    mDefinition("")
{
    spdlog::get("qde")->debug("ShaderObject {}: Getting constructed", identifier(), (void*)this);
}

std::string GLShaderObject::uniforms()
{
    fmt::MemoryWriter out;
    size_t uniformsRemaining = mParameters.size();

    for (auto i = mParameters.begin(); i != mParameters.end(); ++i)
    {
        auto property = (*i).second;
        auto builtin = property.first;
        auto name = fmt::format(builtin.name, mId);

        out << "uniform " << TYPE_TO_BUILTIN_STR[builtin.builtinType] << " " << name <<";";

        if (uniformsRemaining-- != 1) {
            out << "\n";
        }
    }

    return out.str();
}

std::string GLShaderObject::call()
{
    fmt::MemoryWriter out;
    size_t uniformsTotal     = mParameters.size();
    size_t uniformsRemaining = mParameters.size();

    out << mFunctionName << "(";
    for (auto i = mParameters.begin(); i != mParameters.end(); ++i)
    {
        auto property = (*i).second;
        auto builtin = property.first;
        auto name = fmt::format(builtin.name, mId);

        if (uniformsTotal == uniformsRemaining) {
            out << "position - ";
        }

        out << name;

        if (uniformsRemaining-- != 1) {
            out << ", ";
        }
    }
    out << ");";

    spdlog::get("qde")->debug("ShaderObject {}: Calculated output", name());

    return out.str();
}

void GLShaderObject::parseFromFile(const std::string &filename)
{
    spdlog::get("qde")->debug("ShaderObject {}: Parsing file {}", name(), filename);

    std::string shaderSource = Util::fileToString(filename);

    pugi::xml_document doc;
    doc.load_string(shaderSource.c_str());
    pugi::xml_node shaderFunction = doc.child("function");

    mFunctionName = shaderFunction.child_value("name");
    spdlog::get("qde")->debug("ShaderObject {}: Found function {}", name(), mFunctionName);

    pugi::xml_node shaderParameters = shaderFunction.child("parameters");
    for (pugi::xml_node param : shaderParameters) {
        spdlog::get("qde")->debug("ShaderObject {}: Found parameter {}", name(), param.child_value("name"));
        GLShaderBuiltin builtin = {
            fmt::format("{}_{{}}_{}", mName, param.child_value("name")),
            BUILTIN_STR_TO_TYPE[param.child_value("builtin")],
            STR_TO_TYPE[param.child_value("type")]
        };
        addParameterFromBuiltin(builtin);
    }

    mDefinition = shaderFunction.child_value("source");

    spdlog::get("qde")->debug("ShaderObject {}: Parsed file {}", name(), filename);
}

/*
std::string GLShaderObject::parseTuFunction(glsl::astFunction *fct)
{
        spdlog::get("qde")->debug("ShaderObject {}: AST {}: Function: {}", mIdentifier, (void*)mAstTu, fct->name);

        fmt::MemoryWriter out;

        out << kTypes[((glsl::astBuiltin*)fct->returnType)->type];
        out << " ";
        out << fct->name;
        out << "(";

        for (auto parameter : fct->parameters) {
            out << kTypes[((glsl::astBuiltin*)parameter->baseType)->type];
            out << " ";
            out << parameter->name;

            if (parameter != fct->parameters.back()) {
                out << ", ";
            }
        }

        out << ")\n{\n";

        // TODO: Add statements instead of static code
        // for (auto statement : fct->statements) {
        //     out << statement->name();
        //     out << "\n";
        // }
        out << "\t return length(position) - radius;\n";

        out << "}";

        spdlog::get("qde")->debug("ShaderObject {}({}): AST {}: Parsed Function: {}", mIdentifier, (void*)this, (void*)mAstTu, fct->name);

        return out.str();
}

void GLShaderObject::parseTuGlobals(std::vector<glsl::astGlobalVariable *> globals)
{
        for (auto global : globals) {
            std::string type = kTypes[((glsl::astBuiltin*)global->baseType)->type];

            spdlog::get("qde")->debug("ShaderObject {}: AST {}: Global: {} {}", (void*) this, (void*)mAstTu, type, global->name);

            if (type == "float") {
                mUniforms.add<float>(global->name, type, 0.0f);
            }
            else if (type == "vec3") {
                mUniforms.add<Vector3f>(global->name, type, Vector3f::Zero());
            }
            else {
                spdlog::get("qde")->error("Error while parsing global '{}': Unknown type '{}'", global->name, type);
            }
        }
}
*/

void GLShaderObject::addParameterFromBuiltin(const GLShaderBuiltin &builtin)
{
    switch (builtin.builtinType) {
        case GLShaderBuiltinType::FLOAT:
            mParameters.add(builtin.name, builtin, 0.0f);
            break;

        case GLShaderBuiltinType::VEC3:
            mParameters.add(builtin.name, builtin, Vector3f(Vector3f::Zero()));
            break;

        default:
            throw new std::runtime_error("Unknown bultin type");
            break;
    }
    spdlog::get("qde")->debug("ShaderObject {}: Added param from builtin: {}", name(), builtin.name);
}
