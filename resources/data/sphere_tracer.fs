// The MIT License (MIT)
// 
// Copyright (c) 2015-2016 Sven Osterwalder
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// FRAGMENT SHADER

// Vertex shader of a sphere tracer.

#version 330

{{uniform_variables}}
uniform float intensity;
// Global uniform unsigned int variable defining whether to show the distance
// field or not.
uniform int globalShowDistance;
uniform mat4 modelViewProj;

out vec4 fragColor;

// Returns a hash value based on the given number.
float hash(float n) {
    return fract(sin(n)*33753.545383);
}

// Calculates the look at matrix, based on the origin, target and the
// roll of the camera.
//
// Returns a 3x3 matrix.
mat3 calcLookAtMatrix(vec3 origin, vec3 target, float roll) {
    vec3 rr = vec3(sin(roll), cos(roll), 0.0);
    vec3 ww = normalize(target - origin);
    vec3 uu = normalize(cross(ww, rr));
    vec3 vv = normalize(cross(uu, ww));

    return mat3(uu, vv, ww);
}

// Calculates the direction of a ray, based on the given camera matrix,
// screen position and the lens length.
//
// Returns a three-dimensional vector.
vec3 getRay(mat3 cameraMatrix, vec2 screenPosition, float lensLength) {
    vec4 ray = normalize(modelViewProj * vec4(screenPosition, lensLength, 1.0f));
    return vec3(ray.x, ray.y, ray.z);
}

// Calculates the direction of a ray, based on the given origin, target,
// screen position and the lens length.
//
// Returns a three-dimensional vector.
vec3 getRay(vec3 origin, vec3 target, vec2 screenPosition, float lensLength) {
    mat3 cameraMatrix = calcLookAtMatrix(origin, target, 0.0);

    return getRay(cameraMatrix, screenPosition, lensLength);
}

// Calculates the screen position based on the screen size and the
// window-relative coordinates of the current fragment (gl_FragCoord).
//
// Returns a two-dimensional vector.
vec2 squareFrame(vec2 screenSize) {
    vec2 position = 2.0 * (gl_FragCoord.xy / screenSize.xy) - 1.0;
    position.x *= screenSize.x / screenSize.y;

    return position;
}

// Calculates the screen position based on the given screen size and the
// given coordinates.
//
// Returns a two-dimensional vector.
vec2 squareFrame(vec2 screenSize, vec2 coord) {
    vec2 position = 2.0 * (coord.xy / screenSize.xy) - 1.0;
    position.x *= screenSize.x / screenSize.y;

    return position;
}

{{object_definitions}}

// Returns the signed distance to a ruler (-line) with given offset.
// Is used for the visualisation of the distance field.
float ruler(in vec3 position, in float rulerOffset)
{
    return abs(position.y) - rulerOffset;
}


// Returns the signed distance for a substraction of given signed
// distance a to signed distance b.
float subtract(float a, float b)
{
    return max(-b, a);
}

// Returns the signed distance for a merge of given signed
// distance a and signed distance b.
float merge(float a, float b)
{
    return min(a, b);
}

// Returns the signed distance for a intersection  of given signed
// distance a and signed distance b.
float intersect(float a, float b)
{
    return (a > b) ? a : b;
}

// Defines the scene which will be drawn at given position.
float scene1(in vec3 position)
{
    float result = 0.0;

    /*
    // Define dimensions.
    float sphereRadius   = 0.6;
    float sphere3Radius  = 1.0;
    vec3  boxDimension   = vec3(0.8);

    // Define offsets/translations.
    vec3 sphereOffset  = vec3(-2.0, 0.78, -2.0);
    vec3 boxOffset     = vec3(-3.0, 0.78,  0.0);
    vec3 box2Offset    = vec3(-2.0, 0.78, -2.0);

    float plane = plane(position - vec3(0.0));

    float intersection = intersect(
        box(position - boxOffset, vec3(0.8)),
        sphere(position - boxOffset, 1.1)
    );

    float subtraction = subtract(
        box(position - box2Offset, vec3(0.8)),
        sphere(position - box2Offset, sphere3Radius)
    );

    if (globalShowDistance == 1) {
        // Do not show ground plane when the visualisation of the
        // distance function is enabled.
        result = merge(
            sphere(position - sphereOffset, sphereRadius),
            intersection
        );
    }
    else {
        result = merge(
            plane,
            sphere(position - sphereOffset, sphereRadius)
        );
        result = merge(
            result,
            intersection
        );
    }

    result = merge(
        result,
        subtraction
    );
    */

    {{calls}}

    return result;
}

/*
// Returns a simple scene for testing/documenting.
float testScene(in vec3 position)
{
    float result = 0.0;

    float sphere = sphere(position - vec3(0.0, 1.0, 0.0), 1.0);
    float plane = plane(position);

    result = merge(
        plane, sphere
    );

    return result;
}
*/

// Combines the defined scene with the ruler for visualising the
// distance function/field. Note that the input as well as the output
// are three-dimensional vectors, whose third parameter is used as
// identifier.
vec3 combineScenes(vec3 p)
{
    float scene     = scene1(p);
    float ruler     = ruler(p, 0.01);
    float showRuler = ruler < scene ? 1.0 : 0.0;

    return vec3(min(scene, ruler), showRuler, scene);
}

// Calcuates soft shadows for the given ray origin and direction.
//
// Returns a shadow color for the given origin between 0.0 and 1.0.
float calcShadows(in vec3 rayOrigin, in vec3 rayDirection)
{
    float shadow            = 1.0;
    float minimalDistance   = 0.02;
    float maximalDistance   = 2.5;
    float convergePrecision = 0.000001;
    float kShadow           = 8.0;
    float currentDistance   = minimalDistance;

    while (currentDistance < maximalDistance) {
        vec3 ray = rayOrigin + rayDirection * currentDistance;
        float estimatedDistance = scene1(ray);

        if (estimatedDistance < convergePrecision) {
            return 0.0;
        }

        float penumbraFactor = estimatedDistance / currentDistance;
        shadow = min(shadow, kShadow * penumbraFactor);
        currentDistance += estimatedDistance;
    }

    return clamp(shadow, 0.0, 1.0);

}

float calcAO( in vec3 pos, in vec3 nor )
{
    float occ = 0.0;
    float sca = 1.0;
    for( int i=0; i<5; i++ )
    {
        float hr = 0.01 + 0.12*float(i)/4.0;
        vec3 aopos =  nor * hr + pos;
        float dd = scene1( aopos );
        occ += -(dd-hr)*sca;
        sca *= 0.95;
    }
    return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );    
}

// Calculates the lighting for the given position, normal and direction,
// the given light (position and color) respecting the 'material'.
//
// This is mainly applying the phong lighting model inlcuding shadows.
//
// Returns the calculated color as three-dimensional vector.
vec3 calcLighting(in vec3 position, in vec3 normal, in vec3 rayDirection, in vec3 material, in vec3 lightPosition, in vec3 lightColor, in float currentDistance)
{
    vec3 color              = material;

    vec3 lightDirection     = normalize(lightPosition);
    vec3 reflection         = reflect(rayDirection, normal);

    float kOcclusion        = 1.0;
    float occlusion         = calcAO(position, normal);

    float kDirectLight      = 0.1;
    float shadows           = calcShadows(position, lightDirection);
    vec3  direct            = vec3(kDirectLight * shadows);

    vec3  ambientColor      = vec3(0.5, 0.7, 1.0);
    float kAmbient          = 1.2;
    float ambientExponent   = clamp(0.5 + 0.5 * normal.y, 0.0, 1.0);
    vec3  ambient           = kAmbient * ambientExponent * ambientColor * occlusion;

    vec3  diffuseColor      = vec3(1.0, 0.85, 0.55);
    float kDiffuse          = 1.20;
    float diffuseExponent   = clamp(dot(lightDirection, normal), 0.0, 1.0);
    vec3  diffuse           = kDiffuse * diffuseExponent * diffuseColor;
    diffuse                 *= shadows;

    vec3  backgroundColor    = vec3(0.25, 0.25, 0.25);
    float kBackground        = 0.3;
    float backgroundExponent = clamp(dot(normal, normalize(vec3(-lightDirection.x, 0.0, -lightDirection.z))), 0.0, 1.0 ) * clamp(1.0-position.y, 0.0, 1.0);
    vec3  background         = kBackground * backgroundExponent * backgroundColor * occlusion;

    vec3  domainColor        = vec3(0.5, 0.7, 1.0);
    float kExponent          = 0.3;
    float domainExponent     = smoothstep(-0.1, 0.1, reflection.y);
    vec3  domain             = kExponent * domainExponent * domainColor * occlusion;
    domain                   *= shadows;

    vec3 fresnelColor        = vec3(1.0, 1.0, 1.0);
    float kFresnel           = 0.4;
    float fresnelExponent    = pow(clamp(1.0 + dot(normal, rayDirection), 0.0, 1.0), 2.0);
    vec3 fresnel             = kFresnel * fresnelExponent * fresnelColor * occlusion;

    vec3  specularColor     = vec3(1.0, 0.85, 0.55);
    float kSpecular         = 1.2;
    float specularFactor    = 160.0;
    float specularExponent  = pow(clamp(dot(reflection, normal), 0.0, 1.0), specularFactor);
    vec3  specular          = kSpecular * specularExponent * specularColor * diffuseExponent;

    vec3 light              = diffuse + specular + ambient + domain + background + fresnel; // + direct;
    color                   = color * light;
    color                   = mix(color, vec3(0.8, 0.9, 1.0), 1.0 - exp(-0.002 * currentDistance * currentDistance));

    return color;
}

// Calculates the material for the given position and normal.
// Not implemented yet.
vec3 calcMaterial(in vec3 position, in vec3 normal, in float currentDistance)
{
    return 0.45 + 0.3 * sin(vec3(0.05, 0.08, 0.10) * (currentDistance - 10.0));
}

// Calculates the normal vector for given position with respect to a
// certain offset given by epsilon.
vec3 calcNormal(in vec3 position, in float epsilon)
{
    const vec3 v1 = vec3( 1.0, -1.0, -1.0);
    const vec3 v2 = vec3(-1.0, -1.0,  1.0);
    const vec3 v3 = vec3(-1.0,  1.0, -1.0);
    const vec3 v4 = vec3( 1.0,  1.0,  1.0);

    vec3 sceneAtV1 = combineScenes(position + v1 * epsilon);
    vec3 sceneAtV2 = combineScenes(position + v2 * epsilon);
    vec3 sceneAtV3 = combineScenes(position + v3 * epsilon);
    vec3 sceneAtV4 = combineScenes(position + v4 * epsilon);

    float currentDistanceAtV1 = -1.0;
    float currentDistanceAtV2 = -1.0;
    float currentDistanceAtV3 = -1.0;
    float currentDistanceAtV4 = -1.0;


    if (globalShowDistance == 1) {
        currentDistanceAtV1 = sceneAtV1.x;
        currentDistanceAtV2 = sceneAtV2.x;
        currentDistanceAtV3 = sceneAtV3.x;
        currentDistanceAtV4 = sceneAtV4.x;
    }
    else {
        currentDistanceAtV1 = sceneAtV1.z;
        currentDistanceAtV2 = sceneAtV2.z;
        currentDistanceAtV3 = sceneAtV3.z;
        currentDistanceAtV4 = sceneAtV4.z;
    }

    return normalize(v1 * currentDistanceAtV1 +
                     v2 * currentDistanceAtV2 +
                     v3 * currentDistanceAtV3 +
                     v4 * currentDistanceAtV4);
}

// Calculates a 'rule' (a line) for the given distance with the given
// scale.
float calcRule(in float currentDistance, in float scale) {
    return mix(1.0, 0.35, smoothstep(0.6, 1.0, abs(fract(currentDistance * scale) * 2.0 - 1.0)));
}

// Calcualtes the color of a 'rule' at given step size. The color can be
// between 0.0 and 1.0.
vec3 calcRulerColor(float stepSize) {
    stepSize = clamp(log(stepSize + 1.0), 0.0, 1.0);

    return mix(mix(vec3(0.0, 0.1, 1.0), vec3(1.0, 0.1, 0.0), stepSize * 5.0), vec3(1.0), smoothstep(0.2, 0.5, stepSize));
}

// Calcualtes the color between 'rules' at given step size. 
vec3 calcRulerMaterial(float distanceToScene, float currentDistance) {
    float limit = log(currentDistance) / log(10.0);
    float stepSize = pow(10.0, -floor(limit));
    float scale = smoothstep(0.0, 0.33, fract(limit));
    float interpolatedRule = calcRule(distanceToScene, stepSize * 10.0) * mix(calcRule(distanceToScene, stepSize * 100.0), calcRule(distanceToScene, stepSize), scale);

    return mix(calcRulerColor(stepSize * distanceToScene), calcRulerColor(stepSize * distanceToScene * 0.1), scale) * 0.8 * interpolatedRule;
}

// Calculates post processing effects based on given color and screen
// position. This may be used for performing gamma correction, adding
// noise or e.g. vignetting.
vec3 calcPostFx(vec3 color, vec2 screenPosition)
{
    float contrast   = 1.0;
    float saturation = 1.7;
    float brightness = 1.2;

    // Gamma correction
    color = pow(color, vec3(0.4545));

    // color = mix(vec3(.5), mix(vec3(dot(vec3(.2125, .7154, .0721), color*brightness)), color*brightness, saturation), contrast);

    // Noise
    // float noiseFactor = hash(globalTime);
    // vec2 noise        = noiseFactor * screenPosition;
    // color             = clamp(color + noise * 0.1, 0.0, 1.0);

    // Vignetting
    // color *= .4+0.5*pow(40.0*screenPosition.x*screenPosition.y*(1.0-screenPosition.x)*(1.0-screenPosition.y), 0.2 );

    return color;
}

// Casts a ray from given origin i given direction. Stops at given
// maximal distance and after given amount of steps. Maintains given
// precision.
vec3 castRay(in vec3 rayOrigin, in vec3 rayDirection, in float maxDistance, in float prec, in int steps)
{
    float latest          = prec * 2.0;
    float currentDistance = 0.0;
    float result          = -1.0;
    vec3  ray             = vec3(0);
    float ruler           = 0.0;
    vec3 combinedScenes   = vec3(-1.0);

    for(int i = 0; i < steps; i++) {
        if (abs(latest) < prec || currentDistance > maxDistance) {
            continue;
        }

        ray = rayOrigin + rayDirection * currentDistance;
        combinedScenes = combineScenes(ray);

        if (globalShowDistance == 1) {
            latest = combinedScenes.x;
        }
        else {
            latest = combinedScenes.z;
        }

        currentDistance += latest;
    }

    if (currentDistance < maxDistance) {
        result = currentDistance;
    }

    float showRuler     = combinedScenes.y;
    float renderedScene = combinedScenes.z;

    return vec3(result, showRuler, renderedScene);
}

// Performs rendering of a scene beginning at given origin in given ray
// direction. This invokes calculating the normal vector, the material
// as well as the lighting.
vec3 render(in vec3 rayOrigin, in vec3 rayDirection)
{
    // Set background color depending on ray's Y-direction
    vec3  color           = vec3(0.7, 0.9, 1.0) + rayDirection.y * 0.8;

    vec3  res             = castRay(rayOrigin, rayDirection, 100.0, 0.00001, 100);
    float currentDistance = res.x;
    bool  showRuler       = res.y == 1.0;
    float renderedScene   = res.z;
    float minimalDistance = -0.5;

    // Perform further calculation only when the distance is not below
    // the minimal distance (epsilon-factor).
    if (currentDistance > minimalDistance) {
        vec3 position = rayOrigin + currentDistance * rayDirection;
        vec3 normal   = calcNormal(position, 0.000001);

        vec3 material = vec3(0.0);

        if (globalShowDistance == 1) {
            if (showRuler) {
                material = calcRulerMaterial(renderedScene, currentDistance);
            }
            else {
                material = calcMaterial(position, normal, currentDistance);
            }
        }
        else {
            material = calcMaterial(position, normal, currentDistance);
        }

        vec3 light1Color = vec3(0.9, 0.49, 0.83);
        vec3 light1Position = vec3(0.6, 0.7, 1.5);
        color = calcLighting(position, normal, rayDirection, material, light1Position, light1Color, currentDistance);

        vec3 light2Color = vec3(0.43, 0.74, 0.111);
        vec3 light2Position = vec3(10.3, 0.2, 1.8);
        vec3 light2 = calcLighting(position, normal, rayDirection, material, light2Position, light2Color, currentDistance);

        //color = light1 + light2;
    }
    color      = clamp(color, 0.0, 1.0);

    return color;
}

// Main method of the shader.
void main()
{
    // float cameraAngle    = 0.1 * 1.0 + 12.0;
    // float cameraHeight   = 2.0;
    // float cameraPane     = 3.5;
    // float cameraDistance = 3.5;

    // vec3 rayOrigin       = vec3(-1.5 + cameraPane * cos(cameraAngle), 1.0 + cameraHeight, 0.5 + cameraDistance * sin(cameraAngle));
    // vec3 rayTarget       = vec3(0.0, 1.0, 0.0);
    vec3 rayOrigin          = {{cameraOrigin}};
    vec3 rayTarget          = {{cameraTarget}};

    //vec3 rayDirection    = getRay(rayOrigin, rayTarget, screenPosition, 2.0);
    vec2 screenPosition  = squareFrame(vec2(800, 600));
    vec3 rayDirection    = getRay(rayOrigin, rayTarget, screenPosition, {{cameraRoll}});

    vec3 color           = render(rayOrigin, rayDirection);
    color                = calcPostFx(color, screenPosition);

    fragColor.rgb        = color;
    fragColor.a          = 1.0;
}
