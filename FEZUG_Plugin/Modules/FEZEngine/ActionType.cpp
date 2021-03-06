#include "ActionType.hpp"

string FEZEngine::ActionTypeToString(ActionType type, bool includeID) {
    string n = "";
    switch (type) {
    case None: n = "None"; break;
    case Idle: n = "Idle"; break;
    case LookingLeft: n = "LookingLeft"; break;
    case LookingRight: n = "LookingRight"; break;
    case LookingUp: n = "LookingUp"; break;
    case LookingDown: n = "LookingDown"; break;
    case Walking: n = "Walking"; break;
    case Running: n = "Running"; break;
    case Jumping: n = "Jumping"; break;
    case FrontClimbingLadder: n = "FrontClimbingLadder"; break;
    case BackClimbingLadder: n = "BackClimbingLadder"; break;
    case SideClimbingLadder: n = "SideClimbingLadder"; break;
    case CarryIdle: n = "CarryIdle"; break;
    case CarryWalk: n = "CarryWalk"; break;
    case CarryJump: n = "CarryJump"; break;
    case CarrySlide: n = "CarrySlide"; break;
    case CarryEnter: n = "CarryEnter"; break;
    case CarryHeavyIdle: n = "CarryHeavyIdle"; break;
    case CarryHeavyWalk: n = "CarryHeavyWalk"; break;
    case CarryHeavyJump: n = "CarryHeavyJump"; break;
    case CarryHeavySlide: n = "CarryHeavySlide"; break;
    case CarryHeavyEnter: n = "CarryHeavyEnter"; break;
    case DropTrile: n = "DropTrile"; break;
    case DropHeavyTrile: n = "DropHeavyTrile"; break;
    case Throwing: n = "Throwing"; break;
    case ThrowingHeavy: n = "ThrowingHeavy"; break;
    case Lifting: n = "Lifting"; break;
    case LiftingHeavy: n = "LiftingHeavy"; break;
    case Dying: n = "Dying"; break;
    case Suffering: n = "Suffering"; break;
    case Falling: n = "Falling"; break;
    case Bouncing: n = "Bouncing"; break;
    case Flying: n = "Flying"; break;
    case Dropping: n = "Dropping"; break;
    case Sliding: n = "Sliding"; break;
    case Landing: n = "Landing"; break;
    case ReadingSign: n = "ReadingSign"; break;
    case FreeFalling: n = "FreeFalling"; break;
    case CollectingFez: n = "CollectingFez"; break;
    case Victory: n = "Victory"; break;
    case EnteringDoor: n = "EnteringDoor"; break;
    case Grabbing: n = "Grabbing"; break;
    case Pushing: n = "Pushing"; break;
    case SuckedIn: n = "SuckedIn"; break;
    case FrontClimbingVine: n = "FrontClimbingVine"; break;
    case FrontClimbingVineSideways: n = "FrontClimbingVineSideways"; break;
    case SideClimbingVine: n = "SideClimbingVine"; break;
    case BackClimbingVine: n = "BackClimbingVine"; break;
    case BackClimbingVineSideways: n = "BackClimbingVineSideways"; break;
    case WakingUp: n = "WakingUp"; break;
    case OpeningTreasure: n = "OpeningTreasure"; break;
    case OpeningDoor: n = "OpeningDoor"; break;
    case WalkingTo: n = "WalkingTo"; break;
    case Treading: n = "Treading"; break;
    case Swimming: n = "Swimming"; break;
    case Sinking: n = "Sinking"; break;
    case Teetering: n = "Teetering"; break;
    case HurtSwim: n = "HurtSwim"; break;
    case EnteringTunnel: n = "EnteringTunnel"; break;
    case PushingPivot: n = "PushingPivot"; break;
    case EnterDoorSpin: n = "EnterDoorSpin"; break;
    case EnterDoorSpinCarry: n = "EnterDoorSpinCarry"; break;
    case EnterDoorSpinCarryHeavy: n = "EnterDoorSpinCarryHeavy"; break;
    case EnterTunnelCarry: n = "EnterTunnelCarry"; break;
    case EnterTunnelCarryHeavy: n = "EnterTunnelCarryHeavy"; break;
    case RunTurnAround: n = "RunTurnAround"; break;
    case FindingTreasure: n = "FindingTreasure"; break;
    case IdlePlay: n = "IdlePlay"; break;
    case IdleSleep: n = "IdleSleep"; break;
    case IdleLookAround: n = "IdleLookAround"; break;
    case PullUpCornerLedge: n = "PullUpCornerLedge"; break;
    case LowerToCornerLedge: n = "LowerToCornerLedge"; break;
    case GrabCornerLedge: n = "GrabCornerLedge"; break;
    case GrabLedgeFront: n = "GrabLedgeFront"; break;
    case GrabLedgeBack: n = "GrabLedgeBack"; break;
    case PullUpFront: n = "PullUpFront"; break;
    case PullUpBack: n = "PullUpBack"; break;
    case LowerToLedge: n = "LowerToLedge"; break;
    case ShimmyFront: n = "ShimmyFront"; break;
    case ShimmyBack: n = "ShimmyBack"; break;
    case ToCornerFront: n = "ToCornerFront"; break;
    case ToCornerBack: n = "ToCornerBack"; break;
    case FromCornerBack: n = "FromCornerBack"; break;
    case IdleToClimb: n = "IdleToClimb"; break;
    case IdleToFrontClimb: n = "IdleToFrontClimb"; break;
    case IdleToSideClimb: n = "IdleToSideClimb"; break;
    case JumpToClimb: n = "JumpToClimb"; break;
    case JumpToSideClimb: n = "JumpToSideClimb"; break;
    case ClimbOverLadder: n = "ClimbOverLadder"; break;
    case GrabTombstone: n = "GrabTombstone"; break;
    case PivotTombstone: n = "PivotTombstone"; break;
    case LetGoOfTombstone: n = "LetGoOfTombstone"; break;
    case EnteringPipe: n = "EnteringPipe"; break;
    case ExitDoor: n = "ExitDoor"; break;
    case ExitDoorCarry: n = "ExitDoorCarry"; break;
    case ExitDoorCarryHeavy: n = "ExitDoorCarryHeavy"; break;
    case LesserWarp: n = "LesserWarp"; break;
    case GateWarp: n = "GateWarp"; break;
    case SleepWake: n = "SleepWake"; break;
    case ReadTurnAround: n = "ReadTurnAround"; break;
    case EndReadTurnAround: n = "EndReadTurnAround"; break;
    case TurnToBell: n = "TurnToBell"; break;
    case HitBell: n = "HitBell"; break;
    case TurnAwayFromBell: n = "TurnAwayFromBell"; break;
    case CrushHorizontal: n = "CrushHorizontal"; break;
    case CrushVertical: n = "CrushVertical"; break;
    case DrumsIdle: n = "DrumsIdle"; break;
    case DrumsCrash: n = "DrumsCrash"; break;
    case DrumsTom: n = "DrumsTom"; break;
    case DrumsTom2: n = "DrumsTom2"; break;
    case DrumsToss: n = "DrumsToss"; break;
    case DrumsTwirl: n = "DrumsTwirl"; break;
    case DrumsHiHat: n = "DrumsHiHat"; break;
    case VictoryForever: n = "VictoryForever"; break;
    case Floating: n = "Floating"; break;
    case Standing: n = "Standing"; break;
    case StandWinking: n = "StandWinking"; break;
    case IdleYawn: n = "IdleYawn"; break;
    }

    if(includeID) n += Utils::ssprintf(" (%d)", type);

    return n;
}