#pragma once

enum class RenderMethod
{
    INSTANCED = 0,
    MULTIDRAW = 1
};

const char* const RENDER_METHOD_NAMES[] = {
    "Instanced Rendering",
    "MultiDraw Rendering"
};