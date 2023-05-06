#pragma once

#include <cstdint>

#define BIT(n) (0x1U << (n))

#define BQ25601_REG_ISC                         0x00 // Input Source Control
#define BQ25601_REG_ISC_EN_HIZ_MASK             BIT(7)
#define BQ25601_REG_ISC_EN_HIZ_SHIFT            7
#define BQ25601_REG_ISC_EN_ICHG_MON_MASK        (BIT(6) | BIT(5))
#define BQ25601_REG_ISC_EN_ICHG_MON_SHIFT       5
#define BQ25601_REG_ISC_IINDPM_MASK             (BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))
#define BQ25601_REG_ISC_IINDPM_SHIFT            0


#define BQ25601_REG_POC                         0x01 // Power-On Configuration
#define BQ25601_REG_POC_PFM_DIS_MASK            BIT(7)
#define BQ25601_REG_POC_PFM_DIS_SHIFT           7
#define BQ25601_REG_POC_WD_RST_MASK             BIT(6)
#define BQ25601_REG_POC_WD_RST_SHIFT            6
#define BQ25601_REG_POC_CHG_CONFIG_MASK         (BIT(5) | BIT(4))
#define BQ25601_REG_POC_CHG_CONFIG_SHIFT        4
#define BQ25601_REG_POC_SYS_MIN_MASK            (BIT(3) | BIT(2) | BIT(1))
#define BQ25601_REG_POC_SYS_MIN_SHIFT           1
#define BQ25601_REG_POC_MIN_VBAT_SEL_MASK       BIT(0)
#define BQ25601_REG_POC_MIN_VBAT_SEL_SHIFT      0


#define BQ25601_REG_CCC                         0x02 // Charge Current Control
#define BQ25601_REG_CCC_BOOST_LIM_MASK          BIT(7)
#define BQ25601_REG_CCC_BOOST_LIM_SHIFT         7
#define BQ25601_REG_CCC_Q1_FULLON_MASK          BIT(6)
#define BQ25601_REG_CCC_Q1_FULLON_SHIFT         6
#define BQ25601_REG_CCC_ICHG_MASK               (BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))
#define BQ25601_REG_CCC_ICHG_SHIFT              0

#define BQ25601_REG_PCTCC                       0x03 // Pre-charge/Termination Current Cntl
#define BQ25601_REG_PCTCC_IPRECHG_MASK          (BIT(7) | BIT(6) | BIT(5) | BIT(4))
#define BQ25601_REG_PCTCC_IPRECHG_SHIFT         4
#define BQ25601_REG_PCTCC_ITERM_MASK            (BIT(3) | BIT(2) | BIT(1) | BIT(0))
#define BQ25601_REG_PCTCC_ITERM_SHIFT           0

#define BQ25601_REG_CVC                         0x04 // Charge Voltage Control
#define BQ25601_REG_CVC_VREG_MASK               (BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3))
#define BQ25601_REG_CVC_VREG_SHIFT              3
#define BQ25601_REG_CVC_TOPOFF_TIMER_MASK       (BIT(2) | BIT(1))
#define BQ25601_REG_CVC_TOPOFF_TIMER_SHIFT      1
#define BQ25601_REG_CVC_VRECHG_MASK             BIT(0)
#define BQ25601_REG_CVC_VRECHG_SHIFT            0

#define BQ25601_REG_CTTC                        0x05 // Charge Term/Timer Control
#define BQ25601_REG_CTTC_EN_TERM_MASK           BIT(7)
#define BQ25601_REG_CTTC_EN_TERM_SHIFT          7
#define BQ25601_REG_CTTC_WATCHDOG_MASK          (BIT(5) | BIT(4))
#define BQ25601_REG_CTTC_WATCHDOG_SHIFT         4
#define BQ25601_REG_CTTC_EN_TIMER_MASK          BIT(3)
#define BQ25601_REG_CTTC_EN_TIMER_SHIFT         3
#define BQ25601_REG_CTTC_CHG_TIMER_MASK         BIT(2)
#define BQ25601_REG_CTTC_CHG_TIMER_SHIFT        2
#define BQ25601_REG_CTTC_TREG_MASK              BIT(1)
#define BQ25601_REG_CTTC_TREG_SHIFT             1
#define BQ25601_REG_CTTC_JEITA_ISET_MASK        BIT(0)
#define BQ25601_REG_CTTC_JEITA_ISET_SHIFT       0

#define BQ25601_REG_ICTRC                       0x06 // IR Comp/Thermal Regulation Control
#define BQ25601_REG_ICTRC_OVP_MASK              (BIT(7) | BIT(6))
#define BQ25601_REG_ICTRC_OVP_SHIFT             6
#define BQ25601_REG_ICTRC_BOOSTV_MASK           (BIT(5) | BIT(4))
#define BQ25601_REG_ICTRC_BOOSTV_SHIFT          4
#define BQ25601_REG_ICTRC_VINDPM_MASK           (BIT(3) | BIT(2) | BIT(1) | BIT(0))
#define BQ25601_REG_ICTRC_VINDPM_SHIFT              0

#define BQ25601_REG_MOC                         0x07 // Misc. Operation Control
#define BQ25601_REG_MOC_IINDET_EN_MASK          BIT(7)
#define BQ25601_REG_MOC_IINDET_EN_SHIFT         7
#define BQ25601_REG_MOC_TMR2X_EN_MASK           BIT(6)
#define BQ25601_REG_MOC_TMR2X_EN_SHIFT          6
#define BQ25601_REG_MOC_BATFET_DISABLE_MASK     BIT(5)
#define BQ25601_REG_MOC_BATFET_DISABLE_SHIFT    5
#define BQ25601_REG_MOC_JEITA_VSET_MASK         BIT(4)
#define BQ25601_REG_MOC_JEITA_VSET_SHIFT        4
#define BQ25601_REG_MOC_BATFET_DLY_MASK         BIT(3)
#define BQ25601_REG_MOC_BATFET_DLY_SHIFT        3
#define BQ25601_REG_MOC_BATFET_RST_EN_MASK      BIT(2)
#define BQ25601_REG_MOC_BATFET_RST_EN_SHIFT     2
#define BQ25601_REG_MOC_BAT_TRACK_MASK          (BIT(1) | BIT(0))
#define BQ25601_REG_MOC_BAT_TRACK_SHIFT         0

#define BQ25601_REG_SS                          0x08 // System Status
#define BQ25601_REG_SS_VBUS_STAT_MASK           (BIT(7) | BIT(6) | BIT(5))
#define BQ25601_REG_SS_VBUS_STAT_SHIFT          5
#define BQ25601_REG_SS_CHRG_STAT_MASK           (BIT(4) | BIT(3))
#define BQ25601_REG_SS_CHRG_STAT_SHIFT          3
#define BQ25601_REG_SS_PG_STAT_MASK             BIT(2)
#define BQ25601_REG_SS_PG_STAT_SHIFT            2
#define BQ25601_REG_SS_THERM_STAT_MASK          BIT(1)
#define BQ25601_REG_SS_THERM_STAT_SHIFT         1
#define BQ25601_REG_SS_VSYS_STAT_MASK           BIT(0)
#define BQ25601_REG_SS_VSYS_STAT_SHIFT          0

#define BQ25601_REG_F                           0x09 // Fault
#define BQ25601_REG_F_WATCHDOG_FAULT_MASK       BIT(7)
#define BQ25601_REG_F_WATCHDOG_FAULT_SHIFT      7
#define BQ25601_REG_F_BOOST_FAULT_MASK          BIT(6)
#define BQ25601_REG_F_BOOST_FAULT_SHIFT         6
#define BQ25601_REG_F_CHRG_FAULT_MASK           (BIT(5) | BIT(4))
#define BQ25601_REG_F_CHRG_FAULT_SHIFT          4
#define BQ25601_REG_F_BAT_FAULT_MASK            BIT(3)
#define BQ25601_REG_F_BAT_FAULT_SHIFT           3
#define BQ25601_REG_F_NTC_FAULT_MASK            (BIT(2) | BIT(1) | BIT(0))
#define BQ25601_REG_F_NTC_FAULT_SHIFT           0

#define BQ25601_REG_A	                        0x0A // Misc
#define BQ25601_REG_A_VBUS_GD_MASK              BIT(7)
#define BQ25601_REG_A_VBUS_GD_SHIFT             7
#define BQ25601_REG_A_VINDPM_STAT_MASK          BIT(6)
#define BQ25601_REG_A_VINDPM_STAT_SHIFT         6
#define BQ25601_REG_A_IINDPM_STAT_MASK          BIT(5)
#define BQ25601_REG_A_IINDPM_STAT_SHIFT         5
#define BQ25601_REG_A_TOPOFF_ACTIVE_MASK        BIT(3)
#define BQ25601_REG_A_TOPOFF_ACTIVE_SHIFT       3
#define BQ25601_REG_A_ACOV_STAT_MASK            BIT(2)
#define BQ25601_REG_A_ACOV_STAT_SHIFT           2
#define BQ25601_REG_A_VINDPM_INT_MASK           BIT(1)
#define BQ25601_REG_A_VINDPM_INT_SHIFT          1
#define BQ25601_REG_A_IINDPMINT_STAT_MASK       BIT(0)
#define BQ25601_REG_A_IINDPMINT_STAT_SHIFT      0

#define BQ25601_REG_VPRS                        0x0B // Vendor/Part/Revision Status
#define BQ25601_REG_VPRS_REG_RESET_MASK         BIT(7)
#define BQ25601_REG_VPRS_REG_RESET_SHIFT        7
#define BQ25601_REG_VPRS_PN_MASK                (BIT(6) | BIT(5) | BIT(4)| BIT(3))
#define BQ25601_REG_VPRS_PN_SHIFT               3
#define BQ25601_REG_VPRS_PN_25601               0x2
#define BQ25601_REG_VPRS_DEV_RES_MASK           (BIT(1) | BIT(0))
#define BQ25601_REG_VPRS_DEV_RES_SHIFT          0