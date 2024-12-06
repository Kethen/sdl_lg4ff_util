#ifndef __SDL_HAPTIC_H
#define __SDL_HAPTIC_H

#include "sdl_common.h"

typedef void SDL_Haptic;

// fragments of SDL_haptic.h

#define SDL_HAPTIC_CONSTANT   (1u<<0)
#define SDL_HAPTIC_SINE       (1u<<1)
#define SDL_HAPTIC_SQUARE     (1u<<2)
#define SDL_HAPTIC_TRIANGLE   (1u<<3)
#define SDL_HAPTIC_SAWTOOTHUP (1u<<4)
#define SDL_HAPTIC_SAWTOOTHDOWN (1u<<5)
#define SDL_HAPTIC_RAMP       (1u<<6)
#define SDL_HAPTIC_SPRING     (1u<<7)
#define SDL_HAPTIC_DAMPER     (1u<<8)
#define SDL_HAPTIC_INERTIA    (1u<<9)
#define SDL_HAPTIC_FRICTION   (1u<<10)

#define SDL_HAPTIC_LEFTRIGHT    (1u<<11)

#define SDL_HAPTIC_RESERVED1    (1u<<12)
#define SDL_HAPTIC_RESERVED2    (1u<<13)
#define SDL_HAPTIC_RESERVED3    (1u<<14)

#define SDL_HAPTIC_CUSTOM       (1u<<15)

#define SDL_HAPTIC_GAIN       (1u<<16)
#define SDL_HAPTIC_AUTOCENTER (1u<<17)

#define SDL_HAPTIC_STATUS     (1u<<18)
#define SDL_HAPTIC_PAUSE      (1u<<19)

#define SDL_HAPTIC_POLAR      0
#define SDL_HAPTIC_CARTESIAN  1
#define SDL_HAPTIC_SPHERICAL  2
#define SDL_HAPTIC_STEERING_AXIS 3

#define SDL_HAPTIC_INFINITY   4294967295U

typedef struct SDL_HapticDirection
{
    Uint8 type;         /**< The type of encoding. */
    Sint32 dir[3];      /**< The encoded direction. */
} SDL_HapticDirection;

typedef struct SDL_HapticConstant
{
    /* Header */
    Uint16 type;            /**< SDL_HAPTIC_CONSTANT */
    SDL_HapticDirection direction;  /**< Direction of the effect. */

    /* Replay */
    Uint32 length;          /**< Duration of the effect. */
    Uint16 delay;           /**< Delay before starting the effect. */

    /* Trigger */
    Uint16 button;          /**< Button that triggers the effect. */
    Uint16 interval;        /**< How soon it can be triggered again after button. */

    /* Constant */
    Sint16 level;           /**< Strength of the constant effect. */

    /* Envelope */
    Uint16 attack_length;   /**< Duration of the attack. */
    Uint16 attack_level;    /**< Level at the start of the attack. */
    Uint16 fade_length;     /**< Duration of the fade. */
    Uint16 fade_level;      /**< Level at the end of the fade. */
} SDL_HapticConstant;

typedef struct SDL_HapticPeriodic
{
    /* Header */
    Uint16 type;        /**< SDL_HAPTIC_SINE, SDL_HAPTIC_LEFTRIGHT,
                             SDL_HAPTIC_TRIANGLE, SDL_HAPTIC_SAWTOOTHUP or
                             SDL_HAPTIC_SAWTOOTHDOWN */
    SDL_HapticDirection direction;  /**< Direction of the effect. */

    /* Replay */
    Uint32 length;      /**< Duration of the effect. */
    Uint16 delay;       /**< Delay before starting the effect. */

    /* Trigger */
    Uint16 button;      /**< Button that triggers the effect. */
    Uint16 interval;    /**< How soon it can be triggered again after button. */

    /* Periodic */
    Uint16 period;      /**< Period of the wave. */
    Sint16 magnitude;   /**< Peak value; if negative, equivalent to 180 degrees extra phase shift. */
    Sint16 offset;      /**< Mean value of the wave. */
    Uint16 phase;       /**< Positive phase shift given by hundredth of a degree. */

    /* Envelope */
    Uint16 attack_length;   /**< Duration of the attack. */
    Uint16 attack_level;    /**< Level at the start of the attack. */
    Uint16 fade_length; /**< Duration of the fade. */
    Uint16 fade_level;  /**< Level at the end of the fade. */
} SDL_HapticPeriodic;

typedef struct SDL_HapticCondition
{
    /* Header */
    Uint16 type;            /**< SDL_HAPTIC_SPRING, SDL_HAPTIC_DAMPER,
                                 SDL_HAPTIC_INERTIA or SDL_HAPTIC_FRICTION */
    SDL_HapticDirection direction;  /**< Direction of the effect - Not used ATM. */

    /* Replay */
    Uint32 length;          /**< Duration of the effect. */
    Uint16 delay;           /**< Delay before starting the effect. */

    /* Trigger */
    Uint16 button;          /**< Button that triggers the effect. */
    Uint16 interval;        /**< How soon it can be triggered again after button. */

    /* Condition */
    Uint16 right_sat[3];    /**< Level when joystick is to the positive side; max 0xFFFF. */
    Uint16 left_sat[3];     /**< Level when joystick is to the negative side; max 0xFFFF. */
    Sint16 right_coeff[3];  /**< How fast to increase the force towards the positive side. */
    Sint16 left_coeff[3];   /**< How fast to increase the force towards the negative side. */
    Uint16 deadband[3];     /**< Size of the dead zone; max 0xFFFF: whole axis-range when 0-centered. */
    Sint16 center[3];       /**< Position of the dead zone. */
} SDL_HapticCondition;

typedef struct SDL_HapticRamp
{
    /* Header */
    Uint16 type;            /**< SDL_HAPTIC_RAMP */
    SDL_HapticDirection direction;  /**< Direction of the effect. */

    /* Replay */
    Uint32 length;          /**< Duration of the effect. */
    Uint16 delay;           /**< Delay before starting the effect. */

    /* Trigger */
    Uint16 button;          /**< Button that triggers the effect. */
    Uint16 interval;        /**< How soon it can be triggered again after button. */

    /* Ramp */
    Sint16 start;           /**< Beginning strength level. */
    Sint16 end;             /**< Ending strength level. */

    /* Envelope */
    Uint16 attack_length;   /**< Duration of the attack. */
    Uint16 attack_level;    /**< Level at the start of the attack. */
    Uint16 fade_length;     /**< Duration of the fade. */
    Uint16 fade_level;      /**< Level at the end of the fade. */
} SDL_HapticRamp;

typedef struct SDL_HapticLeftRight
{
    /* Header */
    Uint16 type;            /**< SDL_HAPTIC_LEFTRIGHT */

    /* Replay */
    Uint32 length;          /**< Duration of the effect in milliseconds. */

    /* Rumble */
    Uint16 large_magnitude; /**< Control of the large controller motor. */
    Uint16 small_magnitude; /**< Control of the small controller motor. */
} SDL_HapticLeftRight;

typedef struct SDL_HapticCustom
{
    /* Header */
    Uint16 type;            /**< SDL_HAPTIC_CUSTOM */
    SDL_HapticDirection direction;  /**< Direction of the effect. */

    /* Replay */
    Uint32 length;          /**< Duration of the effect. */
    Uint16 delay;           /**< Delay before starting the effect. */

    /* Trigger */
    Uint16 button;          /**< Button that triggers the effect. */
    Uint16 interval;        /**< How soon it can be triggered again after button. */

    /* Custom */
    Uint8 channels;         /**< Axes to use, minimum of one. */
    Uint16 period;          /**< Sample periods. */
    Uint16 samples;         /**< Amount of samples. */
    Uint16 *data;           /**< Should contain channels*samples items. */

    /* Envelope */
    Uint16 attack_length;   /**< Duration of the attack. */
    Uint16 attack_level;    /**< Level at the start of the attack. */
    Uint16 fade_length;     /**< Duration of the fade. */
    Uint16 fade_level;      /**< Level at the end of the fade. */
} SDL_HapticCustom;

typedef union SDL_HapticEffect
{
    /* Common for all force feedback effects */
    Uint16 type;                    /**< Effect type. */
    SDL_HapticConstant constant;    /**< Constant effect. */
    SDL_HapticPeriodic periodic;    /**< Periodic effect. */
    SDL_HapticCondition condition;  /**< Condition effect. */
    SDL_HapticRamp ramp;            /**< Ramp effect. */
    SDL_HapticLeftRight leftright;  /**< Left/Right effect. */
    SDL_HapticCustom custom;        /**< Custom effect. */
} SDL_HapticEffect;

#endif //__SDL_HAPTIC_H
