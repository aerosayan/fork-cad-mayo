/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "graphics_object_ptr.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Aspect_Window.hxx>
#include <V3d_View.hxx>
class Image_PixMap;

namespace Mayo {

struct GraphicsUtils {
    static void V3dView_fitAll(const OccHandle<V3d_View>& view);
    static bool V3dView_hasClipPlane(const OccHandle<V3d_View>& view, const OccHandle<Graphic3d_ClipPlane>& plane);
    static gp_Pnt V3dView_to3dPosition(const OccHandle<V3d_View>& view, double x, double y);

    static void AisContext_eraseObject(
            const OccHandle<AIS_InteractiveContext>& context,
            const OccHandle<AIS_InteractiveObject>& object);
    static void AisContext_setObjectVisible(
            const OccHandle<AIS_InteractiveContext>& context,
            const OccHandle<AIS_InteractiveObject>& object,
            bool on);

    static AIS_InteractiveContext* AisObject_contextPtr(const GraphicsObjectPtr& object);
    static bool AisObject_isVisible(const GraphicsObjectPtr& object);
    static void AisObject_setVisible(const GraphicsObjectPtr& object, bool on);
    static Bnd_Box AisObject_boundingBox(const GraphicsObjectPtr& object);

    static int AspectWindow_width(const OccHandle<Aspect_Window>& wnd);
    static int AspectWindow_height(const OccHandle<Aspect_Window>& wnd);

    static void Gfx3dClipPlane_setCappingHatch(
            const OccHandle<Graphic3d_ClipPlane>& plane, Aspect_HatchStyle hatch);
    static void Gfx3dClipPlane_setNormal(
            const OccHandle<Graphic3d_ClipPlane>& plane, const gp_Dir& n);
    static void Gfx3dClipPlane_setPosition(
            const OccHandle<Graphic3d_ClipPlane>& plane, double pos);

    static bool ImagePixmap_flipY(Image_PixMap& pixmap);
};

} // namespace Mayo
