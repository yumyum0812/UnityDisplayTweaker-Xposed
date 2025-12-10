#pragma once

/* References:
 *  Scripting API: FullScreenMode
 *  https://docs.unity3d.com/ScriptReference/FullScreenMode.html
 */

enum FullScreenMode : int {
    ExclusiveFullScreen = 0,
    FullScreenWindow = 1,
    MaximizedWindow = 2,
    Windowed = 3,
};