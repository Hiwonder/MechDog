#ifndef ACTION_H
#define ACTION_H
typedef struct{
  uint16_t Duration;
  uint16_t Servos[8];
}mech_action;

typedef struct{
  const char* name;
  mech_action* actions;
  uint16_t actionCount;
}ActionsList;

mech_action stand_four_legs[] = {
  {.Duration = 1000, .Servos = {2127,1619,2196,1602,873,1381,804,1398}}
};

mech_action sit_dowm[] = {
  {.Duration = 1000, .Servos = {2100,1000,2300,1900,900,2000,700,1100}}
};

mech_action go_prone[] = {
  {.Duration = 1000, .Servos = {1900,1900,2000,1900,1100,1100,1000,1100}}
};

mech_action stand_two_legs[] = {
    {.Duration = 800, .Servos = {2100, 1000, 2300, 1900, 900, 2000, 700, 1100}},
    {.Duration = 800, .Servos = {2100, 1000, 1500, 1300, 900, 2000, 1500, 1700}},
    {.Duration = 1000, .Servos = {2100, 1000, 2000, 1300, 900, 2000, 1000, 1700}}
};

mech_action two_legs_back[] = {
    {.Duration = 1000, .Servos = {2100, 1000, 2000, 1300, 800, 2000, 1000, 1700}},
    {.Duration = 800, .Servos = {2100, 1000, 1500, 1300, 900, 2000, 1500, 1700}},
    {.Duration = 800, .Servos = {2100, 1000, 2300, 1900, 900, 2000, 700, 1100}},
    {.Duration = 1000, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action handshake[] = {
    {.Duration = 500, .Servos = {1963, 1452, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 500, .Servos = {700, 900, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 400, .Servos = {700, 1200, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 400, .Servos = {700, 900, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 400, .Servos = {700, 1200, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 700, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action scrape_a_bow[] = {
    {.Duration = 800,.Servos = {2100, 1000, 2300, 1900, 900, 2000, 700, 1100}},
    {.Duration = 800,.Servos = {2100, 1000, 1500, 1300, 900, 2000, 1500, 1700}},
    {.Duration = 1000,.Servos = {2100, 1000, 1950, 1300, 800, 2000, 1050, 1700}},
    {.Duration = 700,.Servos = {1500, 1700, 1950, 1300, 1500, 1300, 1050, 1700}},
    {.Duration = 500,.Servos = {2000, 1700, 1950, 1300, 1000, 1300, 1050, 1700}},
    {.Duration = 500,.Servos = {1500, 1700, 1950, 1300, 1500, 1300, 1050, 1700}},
    {.Duration = 500,.Servos = {2000, 1700, 1950, 1300, 1000, 1300, 1050, 1700}},
    {.Duration = 800,.Servos = {2100, 1300, 1950, 1300, 900, 1700, 1050, 1700}},
    {.Duration = 800,.Servos = {2100, 1300, 1500, 1300, 900, 1700, 1500, 1700}},
    {.Duration = 600,.Servos = {2100, 1000, 2300, 1900, 900, 2000, 700, 1100}},
    {.Duration = 800,.Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action nodding_motion[] = {
    {.Duration = 500,.Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}},
    {.Duration = 300,.Servos = {2200, 1800, 2127, 1619, 800, 1200, 873, 1381}},
    {.Duration = 300,.Servos = {2000, 1600, 2127, 1619, 1000, 1400, 873, 1381}},
    {.Duration = 300,.Servos = {2200, 1800, 2127, 1619, 800, 1200, 873, 1381}},
    {.Duration = 300,.Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action boxing[] = {
    {.Duration = 800,.Servos = {2100, 1000, 2300, 1900, 900, 2000, 700, 1100}},
    {.Duration = 800,.Servos = {2100, 1000, 1500, 1300, 900, 2000, 1500, 1700}},
    {.Duration = 800,.Servos = {2100, 1000, 1950, 1300, 800, 2000, 1050, 1700}},
    {.Duration = 500,.Servos = {2200, 1500, 1950, 1300, 1400, 2100, 1050, 1700}},
    {.Duration = 300,.Servos = {1600, 900, 1950, 1300, 800, 1500, 1050, 1700}},
    {.Duration = 300,.Servos = {2200, 1500, 1950, 1300, 1400, 2100, 1050, 1700}},
    {.Duration = 300,.Servos = {1600, 900, 1950, 1300, 800, 1500, 1050, 1700}},
    {.Duration = 500,.Servos = {2100, 1000, 1950, 1300, 800, 2000, 1050, 1700}},
    {.Duration = 800,.Servos = {2100, 1000, 1500, 1300, 900, 2000, 1500, 1700}},
    {.Duration = 800,.Servos = {2100, 1000, 2300, 1900, 900, 2000, 700, 1100}},
    {.Duration = 1000,.Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action stretch_oneself[] = {
    {.Duration = 1000,.Servos = {700, 900, 1500, 1000, 2300, 2100, 1500, 2000}},
    {.Duration = 500,.Servos = {700, 900, 1500, 1000, 2300, 2100, 1500, 2000}},
    {.Duration = 1000,.Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action pee[] = {
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}},
    {.Duration = 500, .Servos = {1903, 1334, 1966, 1334, 832, 1142, 727, 1142}},
    {.Duration = 500, .Servos = {1903, 1334, 2300, 900, 832, 1142, 727, 1142}},
    {.Duration = 500, .Servos = {1903, 1334, 2300, 900, 832, 1142, 727, 1142}},
    {.Duration = 500, .Servos = {1903, 1334, 1966, 1334, 832, 1142, 727, 1142}},
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action press_up[] = {
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}},
    {.Duration = 500, .Servos = {2050, 1000, 2500, 2000, 950, 2000, 500, 1000}},
    {.Duration = 500, .Servos = {2500, 1900, 2500, 2000, 500, 1100, 500, 1000}},
    {.Duration = 500, .Servos = {2050, 1000, 2500, 2000, 950, 2000, 500, 1000}},
    {.Duration = 500, .Servos = {2500, 1900, 2500, 2000, 500, 1100, 500, 1000}},
    {.Duration = 500, .Servos = {2050, 1000, 2500, 2000, 950, 2000, 500, 1000}},
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action left_foot_kick[] = {
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}},
    {.Duration = 400, .Servos = {1963, 1452, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 200, .Servos = {2300, 1900, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 200, .Servos = {1300, 1300, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 300, .Servos = {1963, 1452, 2032, 1452, 877, 1231, 784, 1231}},
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

mech_action right_foot_kick[] = {
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}},
    {.Duration = 400, .Servos = {2099, 1721, 2187, 1721, 1008, 1490, 936, 1490}},
    {.Duration = 200, .Servos = {2099, 1721, 2187, 1721, 700, 1100, 936, 1490}},
    {.Duration = 200, .Servos = {2099, 1721, 2187, 1721, 1800, 1700, 936, 1490}},
    {.Duration = 300, .Servos = {2099, 1721, 2187, 1721, 1008, 1490, 936, 1490}},
    {.Duration = 500, .Servos = {2127, 1619, 2196, 1602, 873, 1381, 804, 1398}}
};

ActionsList aclist[] = {
  {.name = "stand_four_legs", .actions = stand_four_legs, sizeof(stand_four_legs)/sizeof(stand_four_legs[0])},
  {.name = "sit_dowm", .actions = sit_dowm, sizeof(sit_dowm)/sizeof(sit_dowm[0])},
  {.name = "go_prone", .actions = go_prone, sizeof(go_prone)/sizeof(go_prone[0])},
  {.name = "stand_two_legs", .actions = stand_two_legs, sizeof(stand_two_legs)/sizeof(stand_two_legs[0])},
  {.name = "two_legs_back", .actions = two_legs_back, sizeof(two_legs_back)/sizeof(two_legs_back[0])},
  {.name = "handshake", .actions = handshake, sizeof(handshake)/sizeof(handshake[0])},
  {.name = "scrape_a_bow", .actions = scrape_a_bow, sizeof(scrape_a_bow)/sizeof(scrape_a_bow[0])},
  {.name = "nodding_motion", .actions = nodding_motion, sizeof(nodding_motion)/sizeof(nodding_motion[0])},
  {.name = "boxing", .actions = boxing, sizeof(boxing)/sizeof(boxing[0])},
  {.name = "stretch_oneself", .actions = stretch_oneself, sizeof(stretch_oneself)/sizeof(stretch_oneself[0])},
  {.name = "pee", .actions = pee, sizeof(pee)/sizeof(pee[0])},
  {.name = "press_up", .actions = press_up, sizeof(press_up)/sizeof(press_up[0])},
  {.name = "left_foot_kick", .actions = left_foot_kick, sizeof(left_foot_kick)/sizeof(left_foot_kick[0])},
  {.name = "right_foot_kick", .actions = right_foot_kick, sizeof(right_foot_kick)/sizeof(right_foot_kick[0])},
};

#endif