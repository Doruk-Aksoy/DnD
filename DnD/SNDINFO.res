$pitchshiftrange 2

DarkLance/Charge		DLANCHR
DarkLance/Fire1			DLANFIR1
$limit DarkLance/Fire1 0
DarkLance/Fire2			DLANFIR2
$limit DarkLance/Fire2 0
DarkLance/Fire3			DLANFIR3
$limit DarkLance/Fire3 0
$random DarkLance/Fire { DarkLance/Fire1 DarkLance/Fire2 DarkLance/Fire3 }
DarkLance/Consume		DLANCONS
$limit DarkLance/Consume 0
Darklance/AltHit		DLANALTH
DarkLance/AltCurse		DLANALTC
Darklance/AltFire		DLANALTF
Darklance/Affliction	DLANAFFL
$limit Darklance/Affliction 0

Incinerator/Fire		DRAGNFIR
Incinerator/Spin		DRAGNSPN
Incinerator/Charge		DRAGNCHR

CharonBlaster/Idle		CHARIDLE
CharonBlaster/Fire		CHARFIRE
CharonBlaster/Rail		CHARLITE
CharonBlaster/Hit1		CHARHIT1
CharonBlaster/Hit2		CHARHIT2
CharonBlaster/Hit3		CHARHIT3
$random CharonBlaster/Hit { CharonBlaster/Hit1 CharonBlaster/Hit2 CharonBlaster/Hit3 }

VoidWeapon/Fire			VOIWFIRE
VoidWeapon/Idle			VOIWLOOP
VoidWeapon/Zap			VOIWZAP
VoidWeapon/Bzoop		VOIWBZOO
VoidWeapon/Explode		VOIWEXP
VoidWeapon/PreBoom		VOIWPREB
VoidWeapon/BigBoom		VOIWBIGX
VoidWeapon/Charge		VOIWCHR

Gravdis/Idle			GRAVIDLE
Gravdis/Fire			GRAVFIRE
Gravdis/FireAdd			GRAVADD
Gravdis/TimeOut1		GRAVTO1
Gravdis/TimeOut2		GRAVTO2
$random Gravdis/TimeOut { Gravdis/TimeOut1 Gravdis/TimeOut2 }
Gravdis/Fall1			GRAVFAL1
Gravdis/Fall2			GRAVFAL2
$random Gravdis/Fall { Gravdis/Fall1 Gravdis/Fall2 }
Gravdis/Grab1			GRAVGR1
Gravdis/Grab2			GRAVGR2
Gravdis/Grab3			GRAVGR3
Gravdis/Grab4			GRAVGR4
Gravdis/Grab5			GRAVGR5
$random Gravdis/Grab { Gravdis/Grab1 Gravdis/Grab2 Gravdis/Grab3 Gravdis/Grab4 Gravdis/Grab5 }
Gravdis/Drop1			GRAVDR1
Gravdis/Drop2			GRAVDR2
Gravdis/Drop3			GRAVDR3
Gravdis/Drop4			GRAVDR4
Gravdis/Drop5			GRAVDR5
Gravdis/Drop6			GRAVDR6
Gravdis/Drop7			GRAVDR7
Gravdis/Drop8			GRAVDR8
Gravdis/Drop9			GRAVDR9
$random Gravdis/Drop { Gravdis/Drop1 Gravdis/Drop2 Gravdis/Drop3 Gravdis/Drop4 Gravdis/Drop5 Gravdis/Drop6 Gravdis/Drop7 Gravdis/Drop8 Gravdis/Drop9 }
Gravdis/Hit				GRAVHIT
Gravdis/Charge			GRAVCHRG

Crossbow/Explode1		CBOWEX1
Crossbow/Explode2		CBOWEX2
Crossbow/Explode3		CBOWEX3
Crossbow/Explode4		CBOWEX4
$random Crossbow/Explode { Crossbow/Explode1 Crossbow/Explode2 Crossbow/Explode3 Crossbow/Explode4 }
Crossbow/Fire1			CBOWF1
Crossbow/Fire2			CBOWF2
Crossbow/Fire3			CBOWF3
$random Crossbow/Fire { Crossbow/Fire1 Crossbow/Fire2 Crossbow/Fire3 }
Crossbow/Hit1			CBOWHT1
Crossbow/Hit2			CBOWHT2
Crossbow/Hit3			CBOWHT3
Crossbow/Hit4			CBOWHT4
$random Crossbow/Hit { Crossbow/Hit1 Crossbow/Hit2 Crossbow/Hit3 Crossbow/Hit4 }
Crossbow/String1		CBOWST1
Crossbow/String2		CBOWST2
$random Crossbow/String { Crossbow/String1 Crossbow/String2 }
Crossbow/Reload			CBOWREL

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