#include "debug.h"

void ReportPrint(volatile TEffectState &effect) {
  Serial.print("state: ");
  Serial.println(effect.state);
  Serial.print("effectType: ");
  switch (effect.effectType)
  { case 1:
      Serial.println("USB_EFFECT_CONSTANT");
      break;
    case 2:
      Serial.println("USB_EFFECT_RAMP");
      break;
    case 3:
      Serial.println("USB_EFFECT_SQUARE");
      break;
    case 4:
      Serial.println("USB_EFFECT_SINE");
      break;
    case 5:
      Serial.println("USB_EFFECT_TRIANGLE");
      break;
    case 6:
      Serial.println("USB_EFFECT_SAWTOOTHDOWN");
      break;
    case 7:
      Serial.println("USB_EFFECT_SAWTOOTHUP");
      break;
    case 8:
      Serial.println("USB_EFFECT_SPRING");
      break;
    case 9:
      Serial.println("USB_EFFECT_DAMPER");
      break;
    case 10:
      Serial.println("USB_EFFECT_INERTIA");
      break;
    case 11:
      Serial.println("USB_EFFECT_FRICTION");
      break;
    case 12:
      Serial.println("USB_EFFECT_CUSTOM");
      break;
    default:
      break;
  }

  Serial.print("offset: ");
  Serial.println(effect.offset);

  Serial.print("gain: ");
  Serial.println(effect.gain);

  Serial.print("attackLevel: ");
  Serial.println(effect.attackLevel);

  Serial.print("fadeLevel: ");
  Serial.println(effect.fadeLevel);

  Serial.print("magnitude: ");
  Serial.println(effect.magnitude);

  Serial.print("enableAxis: ");
  Serial.println(effect.enableAxis);

  Serial.print("directionX: ");
  Serial.println(effect.directionX);

  Serial.print("directionY: ");
  Serial.println(effect.directionY);


  Serial.print("cpOffset: ");
  Serial.println(effect.cpOffset);

  Serial.print("positiveCoefficient: ");
  Serial.println(effect.positiveCoefficient);


  Serial.print("negativeCoefficient: ");
  Serial.println(effect.negativeCoefficient);
  
  Serial.print("positiveSaturation: ");
  Serial.println(effect.positiveSaturation);

  Serial.print("negativeSaturation: ");
  Serial.println(effect.negativeSaturation);
  Serial.print("deadBand: ");
  Serial.println(effect.deadBand);
  Serial.print("phase: ");
  Serial.println(effect.phase);
  Serial.print("start: ");
  Serial.println(effect.startMagnitude);
  Serial.print("end: ");
  Serial.println(effect.endMagnitude);
  Serial.print("period: ");
  Serial.println(effect.period);
  Serial.print("duration: ");
  Serial.println(effect.duration);
  Serial.print("fadeTime: ");
  Serial.println(effect.fadeTime);
  Serial.print("attackTime: ");
  Serial.println(effect.attackTime);
  Serial.print("elapsedTime: ");
  Serial.println(effect.elapsedTime);

}
