WoT/Idle		WOTIDLE
WoT/Fire		WOTFIRE
WoT/Alt			WOTALT
WoT/Heal		WOTHEAL

Hades/Load		STNGCLOD
Hades/Clipout	STNGCOUT
Hades/Clipin		STNGCIN
Hades/Fire1		STNGFIR1
Hades/Fire2		STNGFIR2
$random Hades/Fire { Hades/Fire1 Hades/Fire2 }
Hades/Add1		STNGADD1
Hades/Add2		STNGADD2
$random Hades/Add { Hades/Add1 Hades/Add2 }

$limit Hades/FireImpact 0
Hades/FireImpact		STNFIRI

$limit Hades/FireFX 0
$random Hades/FireFX { Hades/FireFX1 Hades/FireFX2 Hades/FireFX3 Hades/FireFX4 }

Hades/FireFX1		STNFIR1
Hades/FireFX2		STNFIR2
Hades/FireFX3		STNFIR3
Hades/FireFX4		STNFIR4

$limit Hades/FireFX5 0
Hades/FireFX5		STNFIR5

Rebounder/Fire		REBOFIRE
Rebounder/Hit		REBOHIT
Rebounder/AltFire1	REBOAFI1
Rebounder/AltFire2	REBOAFI2
$random Rebounder/AltFire { Rebounder/AltFire1 Rebounder/AltFire2 }
Rebounder/AltHit	REBOAHIT

FusionBlaster/fire  FUSBFIRE
FusionBlaster/Hit	FUSBHIT
$limit FusionBlaster/fire 0
FusionBlaster/Out	FUSBOUT
FusionBlaster/In	FUSBIN
FusionBlaster/Alt	FUSBALT
FusionBlaster/Hit2	FUSBHIT2

ViperStaff/Fire		VIPRFIR
ViperStaff/Scream	VIPRSCRM
ViperStaff/Charge	VIPRCHR
ViperStaff/Viper1	VIPRSN1
ViperStaff/Viper2	VIPRSN2
$random ViperStaff/Viper { ViperStaff/Viper1 ViperStaff/Viper2 }
ViperStaff/ViperGo	VIPRSATK
Weapons/ViperExp	dsmaceex
Weapons/ViperExp2	VIPREXP

Weapons/FreezerFire FREZFIRE
Freezer/Ambience	LICICEC
Freezer/Particle1	FRZIP1
Freezer/Particle2	FRZIP2
Freezer/Particle3	FRZIP3
Freezer/Particle4	FRZIP4
Freezer/Particle5	FRZIP5
Freezer/Particle6	FRZIP6
$random Freezer/Particle { Freezer/Particle1 Freezer/Particle2 Freezer/Particle3 Freezer/Particle4 Freezer/Particle5 Freezer/Particle6 }
Freezer/Hit1		LICICEH
Freezer/Hit2		LICICEW
$random Freezer/Hit { Freezer/Hit1 Freezer/Hit2 }

Sword/HitWall      SwordWal
DuskMode/Activate	DUSKMODE

$random Sword/Hit {Sword/Hit1 Sword/Hit2 Sword/Hit3 Sword/Hit4}
Sword/Hit1         SwordHt1
Sword/Hit2         SwordHt2
Sword/Hit3         SwordHt3
Sword/Hit4		   SwordHt4

$random Sword/Swing {Sword/Swing1 Sword/Swing2 Sword/Swing3 Sword/Swing4 Sword/Swing5}
Sword/Swing1       SwordSw1
Sword/Swing2       SwordSw2
Sword/Swing3       SwordSw3
Sword/Swing4       SwordSw4
Sword/Swing5       SwordSw5

$random Sword/SWave {Sword/SWave1 Sword/SWave2 Sword/SWave3 Sword/SWave4 Sword/SWave5}
Sword/SWave1       ShokWav1
Sword/SWave2       ShokWav2
Sword/SWave3       ShokWav3
Sword/SWave4       ShokWav4
Sword/SWave5       ShokWav5

Vorpal/FireHit		EXPLODE
Vorpal/Swing		VORPSWNG
Vorpal/Fire			VORPPOW
$limit Vorpal/Fire 0
Vorpal/FireSee		VORPGO
$limit Vorpal/FireSee 0

AssaultRifle/Fire		ARFLFIR
AssaultRifle/In			ARFLIN
AssaultRifle/Out		ARFLOUT

Deadlock/Fire			DLOCKF
Deadlock/Insert			DLOCKI
Deadlock/Pump			DLOCKP

weapons/cryobowfire     DSBOWFIR
weapons/cryobowshot     DSBOWSHT
weapons/cryobowalt     	DSBOWALT
weapons/cryobowloop		DSBOWLP
$limit weapons/cryobowloop 0
weapons/cryobowreload   DSBOWREL
weapons/cryobowhit      DSBOWHIT
weapons/cryobowflyby    DSBOWFLY

PlasmaCannon/Fire		PCANFIR
PlasmaCannon/Hit		PCANHIT
PlasmaCannon/In			PCANIN
PlasmaCannon/Out		PCANOUT
PlasmaCannon/Charge		PCANCHR

Shocker/Fire			SHCKRFR
Shocker/Fire2			SHCKRF2
Shocker/Smokes			SHCKRSM

Templar/Fire			TPLFIR
Templar/In				TPLIN
Templar/Out				TPLOUT

Weapons/RiotgunFire		SW30SHOT
Weapons/RiotgunLoad		SW30LOAD
Weapons/ShellCasing		SW30SHLL
Weapons/RiotgunUp		SW30RTUP

WeaponGL/Fire					WEPGLFR
WeaponGL/In						WEPGLIN
WeaponGL/Open					WEPGLOPN
WeaponGL/Close					WEPGLCLS
WeaponGL/40MMHExp				40MMHEXP
WeaponGL/40MMHBounce			40MMHBNC
WeaponGL/40MMHHit				40MMHHIT
WeaponGL/40MMSonicExp			40MMSEXP
WeaponGL/40MMSonicBounce		40MMSBNC

HeavyGL/Fire					HGLFIRE
HeavyGL/Explode					HGLEXP
HeavyGL/Cock					HGLCOCK
HeavyGL/Bounce1					HGLBN1
HeavyGL/Bounce2					HGLBN2
$random HeavyGL/Bounce { HeavyGL/Bounce1 HeavyGL/Bounce2 }
HeavyGL/ShrapnelBounce1			HGLSBN1
HeavyGL/ShrapnelBounce2			HGLSBN2
$random HeavyGL/ShrapnelBounce { HeavyGL/ShrapnelBounce1 HeavyGL/ShrapnelBounce2 }
HeavyGL/ShrapnelHit				HGLSFWT
HeavyGL/ShrapnelFlesh			HGLSFHT

AcidRifle/Hit				ACIDHIT
$limit AcidRifle/Hit 0
$volume AcidRifle/Hit 0.75
AcidRifle/In				ACIDREL
AcidRifle/Out				ACIDOUT
AcidRifle/Fire1				ACRFIR1
AcidRifle/Fire2				ACRFIR2
$random AcidRifle/Fire { AcidRifle/Fire1 AcidRifle/Fire2 }
AcidRifle/BoltFire1			ACIDBLF1
AcidRifle/BoltFire2			ACIDBLF2
AcidRifle/BoltFire3			ACIDBLF3
$random AcidRifle/BoltFire { AcidRifle/BoltFire1 AcidRifle/BoltFire2 AcidRifle/BoltFire3 }
AcidRifle/BoltHit			ACIDBLHT
AcidRifle/BoltExplode		ACIDBLXP
AcidRifle/BoltCloud			ACIDBLCL

MeteorLauncher/Fire			METRFIRE
MeteorLauncher/Charge		METRCHRG
MeteorLauncher/Bounce		METRBNC
MeteorLauncher/Hit			DSTEXP1
MeteorLauncher/HitSmall		METRHTS

Flamethrower/Fire			FMTRFIRE
Flamethrower/Hit			FMTRHIT
Flamethrower/Flame			FMTRFLAM
FlameThrower/In				FMTRIN
FlameThrower/Out			FMTROUT
Flamethrower/FuelBounce		FMTRBNC

LightningGun/Fire			LGFIRE
LightningGun/Loop			LGLOOP
LightningGun/FireEnd		LGEND

IonCannon/Fire			IOCANFIR
$limit IonCannon/Fire 0
IonCannon/Spin			IOCANSPN
IonCannon/SpinLoop		IOCANSPL
IonCannon/Slowdown		IOCANSLW
IonCannon/Hit			IOCANHIT
$limit IonCannon/Hit 0

ThunderStaff/Fire		APOTFIRE
ThunderStaff/Impact		APOTHIT2
ThunderStaff/Loop		APOTLOOP
ThunderStaff/Thunder	APOTTHND
ThunderStaff/Alt		APOTALT
$Volume ThunderStaff/Loop 0.0875
$limit ThunderStaff/Thunder 0

Shells/Flechette		FLECHFIR
Shells/FlechetteRic		FLECRIC
Shells/Slug				SHSLUGFR
Shells/Pierce			PIERCFIR
Shells/Shock			ELECTFIR
Shells/Shock2			ELECFIR2

Shells/ShockHit1		ELECHIT1
Shells/ShockHit2		ELECHIT2
Shells/ShockHit3		ELECHIT3
Shells/ShockHit4		ELECHIT4

$random Shells/ShockHit { Shells/ShockHit1 Shells/ShockHit2 Shells/ShockHit3 Shells/ShockHit4 }

AmmoType/Switch			AMMOSWTC