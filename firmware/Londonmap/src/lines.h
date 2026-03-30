#pragma once

#include <stations.h>

// ============================================================================
// BAKERLOO LINE  (single trunk — no branches)
// ============================================================================
const uint16 LINE_BAKERLOO[] = {
    ELEPHANT_CASTLE_A,
    LAMBETH_NORTH,
    WATERLOO_A,
    EMBANKMENT_A,
    CHARING_CROSS,
    PICCADILLY_CIRCUS,
    OXFORD_CIRCUS,
    REGENTS_PARK,
    BAKER_STREET_A,
    MARYLEBONE,
    EDGEWARE_ROAD_A,
    PADDINGTON,
    WARWICK_AVENUE,
    MAIDA_VALE,
    KILBURN_PARK,
    QUEENS_PARK,
    KENSAL_GREEN,
    WILLESDEN_JUNCTION,
    HARLESDEN,
    STONEBRIDGE_PARK,
    WEMBLEY_CENTRAL,
    NORTH_WEMBLEY,
    SOUTH_KENTON,
    KENTON,
    HARROW_WEALDSTONE};

// ============================================================================
// CENTRAL LINE
//   A – Main trunk  West Ruislip → Epping  (direct across Leytonstone)
//   B – Ealing Broadway branch  (Ealing Broadway → North Acton)
//   C – Hainault loop  (Leytonstone → Woodford via Hainault)
// ============================================================================
const uint16 LINE_CENTRAL_A[] = {
    WEST_RUISLIP,
    RUISLIP_GARDENS,
    SOUTH_RUISLIP,
    NORTHOLT,
    GREENFORD,
    PERIVALE,
    HANGER_LANE,
    NORTH_ACTON,
    EAST_ACTON,
    WHITE_CITY,
    SHEPHERDS_BUSH_A,
    HOLLAND_PARK,
    QUEENSWAY,
    LANCASTER_GATE, // NOTE: NOTTING_HILL_GATE shares this same LED (230) on the PCB
    MARBLE_ARCH,
    BOND_STREET_A,
    OXFORD_CIRCUS,
    TOTTENHAM_COURT_ROAD_A,
    HOLBORN,
    CHANCERY_LANE,
    ST_PAULS,
    BANK_A,
    LIVERPOOL_STREET_A,
    BETHNAL_GREEN_CENTRAL_LINE,
    MILE_END,
    STRATFORD,
    LEYTON,
    LEYTONSTONE,
    WOODFORD,
    BUCKHURST_HILL,
    LOUGHTON,
    DEBDEN,
    THEYDON_BOIS,
    EPPING};

const uint16 LINE_CENTRAL_B[] = {
    EALING_BROADWAY,
    WEST_ACTON,
    NORTH_ACTON};

const uint16 LINE_CENTRAL_C[] = {
    LEYTONSTONE,
    WANSTEAD,
    REDBRIDGE,
    GANTS_HILL,
    NEWBURY_PARK,
    BARKINGSIDE,
    FAIRLOP,
    HAINAULT,
    GRANGE_HILL,
    CHIGWELL,
    RODING_VALLEY,
    WOODFORD};

// ============================================================================
// CIRCLE LINE  (complete clockwise loop — no branches)
// ============================================================================
const uint16 LINE_CIRCLE[] = {
    EDGEWARE_ROAD_A,
    PADDINGTON,
    BAYSWATER,
    NOTTING_HILL_GATE,
    HIGH_STREET_KENSINGTON,
    GLOUCESTER_ROAD_A,
    SOUTH_KENSINGTON_A,
    SLOANE_SQUARE,
    VICTORIA_A,
    ST_JAMESS_PARK,
    WESTMINSTER_A,
    EMBANKMENT_A,
    TEMPLE,
    BLACKFRIARS,
    MANSION_HOUSE,
    CANNON_STREET,   // UNMAPPED_LED — no physical LED
    MONUMENT,        // UNMAPPED_LED
    TOWER_HILL,
    ALDGATE,
    LIVERPOOL_STREET_A,
    MOORGATE,
    BARBICAN,        // UNMAPPED_LED
    FARRINGDON,      // UNMAPPED_LED
    KINGS_CROSS_ST_PANCRAS_A,
    EUSTON_SQUARE,
    GREAT_PORTLAND_STREET,
    BAKER_STREET_A};

// ============================================================================
// DISTRICT LINE
//   A – Main trunk   Upminster → Wimbledon
//   B – Richmond branch   Earl's Court → Richmond
//   C – Ealing Broadway branch   Earl's Court → Ealing Broadway
// ============================================================================
const uint16 LINE_DISTRICT_A[] = {
    UPMINSTER_A,
    UPMINSTER_BRIDGE,
    HORNCHURCH,
    ELM_PARK,
    DAGENHAM_EAST,
    DAGENHAM_HEATHWAY,
    BECONTREE,
    UPNEY,
    BARKING_A,
    EAST_HAM,
    UPTON_PARK,
    PLAISTOW,
    WEST_HAM_A,
    BROMLEY_BY_BOW,
    BOW_ROAD,
    MILE_END,
    STEPNEY_GREEN,
    WHITECHAPEL_A,
    ALDGATE_EAST,
    TOWER_HILL,
    MONUMENT,        // UNMAPPED_LED
    CANNON_STREET,   // UNMAPPED_LED
    MANSION_HOUSE,
    BLACKFRIARS,
    TEMPLE,
    EMBANKMENT_A,
    WESTMINSTER_A,
    ST_JAMESS_PARK,
    VICTORIA_A,
    SLOANE_SQUARE,
    SOUTH_KENSINGTON_A,
    GLOUCESTER_ROAD_A,
    EARLS_COURT_A,
    WEST_BROMPTON_A,
    FULHAM_BROADWAY,
    PARSONS_GREEN,
    PUTNEY_BRIDGE,
    EAST_PUTNEY,
    SOUTHFIELDS,
    WIMBLEDON_PARK_A,
    WIMBLEDON_A};

const uint16 LINE_DISTRICT_B[] = {
    EARLS_COURT_A,
    BARONS_COURT,
    HAMMERSMITH_A,
    RAVENSCOURT_PARK,
    STAMFORD_BROOK,
    TURNHAM_GREEN_A,
    CHISWICK_PARK,
    GUNNERSBURY_A,
    KEW_GARDENS_A,
    RICHMOND_A};

const uint16 LINE_DISTRICT_C[] = {
    EARLS_COURT_A,
    BARONS_COURT,
    HAMMERSMITH_A,
    RAVENSCOURT_PARK,
    STAMFORD_BROOK,
    TURNHAM_GREEN_A,
    ACTON_TOWN_A,
    EALING_COMMON,
    EALING_BROADWAY};

// ============================================================================
// HAMMERSMITH & CITY LINE  (single trunk — no branches)
// ============================================================================
const uint16 LINE_HAMMERSMITH_AND_CITY[] = {
    HAMMERSMITH_A,
    GOLDHAWK_ROAD,
    SHEPHERDS_BUSH_MARKET,
    WOOD_LANE,
    LATIMER_ROAD,
    LADBROKE_GROVE,
    WESTBOURNE_PARK,
    PADDINGTON,
    EDGEWARE_ROAD_A,
    BAKER_STREET_A,
    GREAT_PORTLAND_STREET,
    EUSTON_SQUARE,
    KINGS_CROSS_ST_PANCRAS_A,
    FARRINGDON,      // UNMAPPED_LED
    BARBICAN,        // UNMAPPED_LED
    MOORGATE,
    LIVERPOOL_STREET_A,
    WHITECHAPEL_A,
    BARKING_A};

// ============================================================================
// JUBILEE LINE  (single trunk — no branches)
// ============================================================================
const uint16 LINE_JUBILEE[] = {
    STANMORE,
    CANONS_PARK,
    QUEENSBURY,
    KINGSBURY,
    WEMBLEY_PARK,
    NEASDEN,
    DOLLIS_HILL,
    WILLESDEN_GREEN,
    KILBURN,
    WEST_HAMPSTEAD,
    FINCHLEY_ROAD_A,
    SWISS_COTTAGE,
    ST_JOHNS_WOOD,
    BAKER_STREET_A,
    BOND_STREET_A,
    GREEN_PARK,
    WESTMINSTER_A,
    WATERLOO_A,
    SOUTHWARK,
    LONDON_BRIDGE_A,
    BERMONDSEY,
    CANADA_WATER,
    CANARY_WHARF_A,
    NORTH_GREENWICH,
    CANNING_TOWN_A,
    WEST_HAM_A,
    STRATFORD};

// ============================================================================
// METROPOLITAN LINE
//   A – Main trunk   Aldgate → Amersham
//   B – Chesham spur   Chalfont & Latimer → Chesham
//   C – Watford branch   Moor Park → Watford
// ============================================================================
const uint16 LINE_METROPOLITAN_A[] = {
    ALDGATE,
    LIVERPOOL_STREET_A,
    MOORGATE,
    BARBICAN,        // UNMAPPED_LED
    FARRINGDON,      // UNMAPPED_LED
    KINGS_CROSS_ST_PANCRAS_A,
    EUSTON_SQUARE,
    GREAT_PORTLAND_STREET,
    BAKER_STREET_A,
    FINCHLEY_ROAD_A,
    WEMBLEY_PARK,
    PRESTON_ROAD,
    NORTHWICK_PARK,
    HARROW_ON_THE_HILL,
    NORTH_HARROW,
    PINNER,
    NORTHWOOD_HILLS,
    NORTHWOOD,
    MOOR_PARK,
    RICKMANSWORTH,
    CHORLEYWOOD,
    CHALFONT_LATIMER,
    AMERSHAM};

const uint16 LINE_METROPOLITAN_B[] = {
    CHALFONT_LATIMER,
    CHESHAM};

const uint16 LINE_METROPOLITAN_C[] = {
    MOOR_PARK,
    CROXLEY,
    WATFORD};

// ============================================================================
// NORTHERN LINE
//   A – Morden → Edgware  (via Charing Cross branch south, Edgware north)
//   B – Battersea → High Barnet  (via Bank branch, High Barnet north)
//   C – Mill Hill East spur  (Finchley Central → Mill Hill East)
// ============================================================================
const uint16 LINE_NORTHERN_A[] = {
    MORDEN,
    SOUTH_WIMBLEDON,
    COLLIERS_WOOD,
    TOOTING_BROADWAY,
    TOOTING_BEC,
    BALHAM,
    CLAPHAM_SOUTH,
    CLAPHAM_COMMON,
    CLAPHAM_NORTH,
    STOCKWELL,
    OVAL,
    KENNINGTON_A,
    WATERLOO_A,      // UNMAPPED_LED
    EMBANKMENT_A,
    CHARING_CROSS,
    LEICESTER_SQUARE,
    TOTTENHAM_COURT_ROAD_A,
    GOODGE_STREET,
    WARREN_STREET,
    EUSTON_A,
    MORNINGTON_CRESCENT,
    CAMDEN_TOWN,
    CHALK_FARM,
    BELSIZE_PARK,
    HAMPSTEAD,
    GOLDERS_GREEN,
    BRENT_CROSS,
    HENDON_CENTRAL,
    COLINDALE,
    BURNT_OAK,
    EDGEWARE};

const uint16 LINE_NORTHERN_B[] = {
    BATTERSEA_POWER_STATION,
    NINE_ELMS,
    KENNINGTON_A,
    ELEPHANT_CASTLE_B,
    BOROUGH,
    LONDON_BRIDGE_A,
    BANK_A,
    MOORGATE,
    OLD_STREET,
    ANGEL,
    EUSTON_A,
    CAMDEN_TOWN,
    KENTISH_TOWN,
    TUFNELL_PARK,
    ARCHWAY,
    HIGHGATE,
    EAST_FINCHLEY,
    FINCHLEY_CENTRAL,
    WEST_FINCHLEY,
    WOODSIDE_PARK,
    TOTTERIDGE_WHETSTONE,
    HIGH_BARNET};

const uint16 LINE_NORTHERN_C[] = {
    FINCHLEY_CENTRAL,
    MILL_HILL_EAST};

// ============================================================================
// PICCADILLY LINE
//   A – Main trunk   Cockfosters → Heathrow T2&3 + T5
//   B – Terminal 4 loop   Hatton Cross → T4 → T2&3
//   C – Uxbridge branch   Acton Town → Uxbridge
// ============================================================================
const uint16 LINE_PICCADILLY_A[] = {
    COCKFOSTERS,
    OAKWOOD,
    SOUTHGATE,
    ARNOS_GROVE,
    BOUNDS_GREEN,
    WOOD_GREEN,
    TURNPIKE_LANE,
    MANOR_HOUSE,
    FINSBURY_PARK_A,
    ARSENAL,
    HOLLOWAY_ROAD,
    CALEDONIAN_ROAD,
    KINGS_CROSS_ST_PANCRAS_A,
    RUSSELL_SQUARE_A,
    HOLBORN,
    COVENT_GARDEN,
    LEICESTER_SQUARE,
    PICCADILLY_CIRCUS,
    GREEN_PARK,
    HYDE_PARK_CORNER,
    KNIGHTSBRIDGE,
    SOUTH_KENSINGTON_A,
    GLOUCESTER_ROAD_A,
    EARLS_COURT_A,
    BARONS_COURT,
    HAMMERSMITH_A,
    ACTON_TOWN_A,
    SOUTH_EALING,
    NORTHFIELDS,
    BOSTON_MANOR,
    OSTERLEY,
    HOUNSLOW_EAST,
    HOUNSLOW_CENTRAL,
    HOUNSLOW_WEST,
    HATTON_CROSS,
    HEATHROW_TERMINALS_2_3_A,
    HEATHROW_TERMINAL_5_A};

const uint16 LINE_PICCADILLY_B[] = {
    HATTON_CROSS,
    HEATHROW_TERMINAL_4_A,
    HEATHROW_TERMINALS_2_3_A};

const uint16 LINE_PICCADILLY_C[] = {
    ACTON_TOWN_A,
    NORTH_EALING,
    PARK_ROYAL,
    ALPERTON,
    SUDBURY_TOWN,
    SUDBURY_HILL,
    SOUTH_HARROW,
    RAYNERS_LANE,
    EASTCOTE,
    RUISLIP_MANOR,
    RUISLIP,
    ICKENHAM,
    UXBRIDGE};

// ============================================================================
// VICTORIA LINE  (single trunk — no branches)
// ============================================================================
const uint16 LINE_VICTORIA[] = {
    WALTHAMSTOW_CENTRAL_A,
    BLACKHORSE_ROAD_A,
    TOTTENHAM_HALE,
    SEVEN_SISTERS,
    FINSBURY_PARK_A,
    HIGHBURY_ISLINGTON_A,
    KINGS_CROSS_ST_PANCRAS_A,
    EUSTON_A,
    WARREN_STREET,
    OXFORD_CIRCUS,
    GREEN_PARK,
    VICTORIA_A,
    PIMLICO,
    VAUXHALL,
    STOCKWELL,
    BRIXTON};

// ============================================================================
// WATERLOO & CITY LINE  (single trunk — no branches)
// ============================================================================
const uint16 LINE_WATERLOO_AND_CITY[] = {
    WATERLOO_A,
    BANK_A};

// ============================================================================
// DLR  (Docklands Light Railway)
//   A – Bank/Tower Gateway → Beckton  (main east trunk)
//   B – Woolwich Arsenal branch  (from Canning Town)
//   C – Stratford → Lewisham  (via Isle of Dogs)
//   D – Stratford High Street → West Ham  (northward spur)
// ============================================================================
const uint16 LINE_DLR_A[] = {
    BANK_A,
    TOWER_GATEWAY,
    SHADWELL_A,
    LIMEHOUSE,
    WESTFERRY,
    POPLAR,
    BLACKWALL,
    EAST_INDIA,
    CANNING_TOWN_A,
    ROYAL_VICTORIA,
    CUSTOM_HOUSE_FOR_EXCEL_A,
    PRINCE_REGENT,
    ROYAL_ALBERT,
    BECKTON_PARK,
    CYPRUS,
    GALLIONS_REACH,
    BECKTON};

const uint16 LINE_DLR_B[] = {
    CANNING_TOWN_A,
    WEST_SILVERTOWN,
    PONTOON_DOCK,
    LONDON_CITY_AIRPORT,
    KING_GEORGE_V,
    WOOLWICH_ARSENAL_A};

const uint16 LINE_DLR_C[] = {
    STRATFORD,
    STRATFORD_INTERNATIONAL,
    PUDDING_MILL_LANE,
    BOW_CHURCH,
    DEVONS_ROAD,
    LANGDON_PARK,
    ALL_SAINTS,
    POPLAR,
    WEST_INDIA_QUAY_A,
    CANARY_WHARF_B,
    HERON_QUAYS,
    SOUTH_QUAY,
    CROSSHARBOUR,
    MUDCHUTE,
    ISLAND_GARDENS,
    CUTTY_SARK_FOR_MARITIME_GREENWICH,
    GREENWICH_A,
    DEPTFORD_BRIDGE,
    ELVERSON_ROAD,
    LEWISHAM};

const uint16 LINE_DLR_D[] = {
    STRATFORD_HIGH_STREET,
    ABBEY_ROAD,
    WEST_HAM_A};

// ============================================================================
// LONDON OVERGROUND
//   A – Mildmay (North London Line)   Richmond → Stratford
//   B – Windrush (East London Line)   Highbury & Islington → Crystal Palace
//   C – Suffragette (Gospel Oak – Barking)
//   D – Lioness (Watford DC line)   Watford Junction → Queens Park
// ============================================================================
const uint16 LINE_OVERGROUND_A[] = {
    RICHMOND_A,
    CLAPHAM_JUNCTION_A,
    SHEPHERDS_BUSH_A,
    WILLESDEN_JUNCTION,
    KENSAL_RISE,
    BRONDESBURY_PARK,
    BRONDESBURY,
    WEST_HAMPSTEAD,
    HAMPSTEAD_HEATH,
    KENTISH_TOWN_WEST,
    CAMDEN_ROAD,
    CALEDONIAN_ROAD_BARNSBURY,
    HIGHBURY_ISLINGTON_A,
    CANONBURY,
    DALSTON_KINGSLAND,
    HACKNEY_CENTRAL,
    HOMERTON,
    HACKNEY_WICK,
    STRATFORD};

const uint16 LINE_OVERGROUND_B[] = {
    HIGHBURY_ISLINGTON_A,
    DALSTON_JUNCTION,
    SHOREDITCH_HIGH_STREET,
    SHADWELL_A,
    WAPPING,
    ROTHERHITHE,
    CANADA_WATER,
    SURREY_QUAYS,
    NEW_CROSS_GATE,
    NEW_CROSS,
    CRYSTAL_PALACE};

const uint16 LINE_OVERGROUND_C[] = {
    GOSPEL_OAK_A,
    CROUCH_HILL,
    UPPER_HOLLOWAY,
    HARRINGAY_GREEN_LANES,
    SOUTH_TOTTENHAM,
    BLACKHORSE_ROAD_A,
    WALTHAMSTOW_QUEENS_ROAD,
    LEYTON_MIDLAND_ROAD,
    LEYTONSTONE_HIGH_ROAD,
    WANSTEAD_PARK,
    WOODGRANGE_PARK,
    FOREST_GATE,
    BARKING_A};

const uint16 LINE_OVERGROUND_D[] = {
    WATFORD_JUNCTION,
    WATFORD_HIGH_STREET,
    BUSHEY,
    CARPENDERS_PARK,
    HATCH_END,
    HEADSTONE_LANE,
    HARROW_WEALDSTONE,
    KENTON,
    SOUTH_KENTON,
    NORTH_WEMBLEY,
    WEMBLEY_CENTRAL,
    STONEBRIDGE_PARK,
    HARLESDEN,
    WILLESDEN_JUNCTION,
    KENSAL_GREEN,
    QUEENS_PARK};

// ============================================================================
// ELIZABETH LINE
//   A – Main trunk   Reading → Shenfield  (via central section)
//   B – Heathrow branch   T5 → T4 → T2&3 → Hayes & Harlington
// ============================================================================
const uint16 LINE_ELIZABETH_A[] = {
    READING,
    TWYFORD,
    TAPLOW,
    BURNHAM,
    SLOUGH,
    LANGLEY,
    WEST_DRAYTON,
    HAYES_HARLINGTON,
    SOUTHALL,
    HANWELL,
    WEST_EALING,
    EALING_BROADWAY,
    ACTON_MAIN_LINE,
    PADDINGTON,
    TOTTENHAM_COURT_ROAD_A,
    FARRINGDON,      // UNMAPPED_LED
    LIVERPOOL_STREET_A,
    WHITECHAPEL_A,
    CANARY_WHARF_A,
    CUSTOM_HOUSE_FOR_EXCEL_A,
    WOOLWICH,
    ABBEY_WOOD,
    STRATFORD,
    MARYLAND,
    FOREST_GATE,
    MANOR_PARK,
    ILFORD,
    SEVEN_KINGS,
    GOODMAYES,
    CHADWELL_HEATH,
    ROMFORD,
    GIDEA_PARK,
    HAROLD_WOOD,
    BRENTWOOD,
    SHENFIELD};

const uint16 LINE_ELIZABETH_B[] = {
    HEATHROW_TERMINAL_5_A,
    HEATHROW_TERMINAL_4_A,
    HEATHROW_TERMINALS_2_3_A,
    HAYES_HARLINGTON};

// ============================================================================
// LONDON TRAMLINK
//   A – Wimbledon → New Addington  (via Mitcham, Croydon, Addington branch)
//   B – Wimbledon → Beckenham Junction  (via Croydon, Arena branch)
//   C – Wimbledon → Elmers End  (via Croydon, Addiscombe branch)
// ============================================================================
const uint16 LINE_TRAMS_A[] = {
    WIMBLEDON_A,
    DUNDONALD_ROAD,
    MERTON_PARK,
    WIMBLEDON_CHASE,
    SOUTH_MERTON,
    MORDEN_ROAD,
    PHIPPS_BRIDGE,
    BELGRAVE_WALK,
    MITCHAM,
    MITCHAM_JUNCTION_A,
    BEDDINGTON_LANE,
    AMPERE_WAY,
    THERAPIA_LANE,
    WADDON_MARSH,
    CENTRALE,
    WEST_CROYDON_A,
    WELLESLEY_ROAD,
    GEORGE_STREET,
    CHURCH_STREET,
    EAST_CROYDON_A,
    SANDILANDS,
    LLOYD_PARK,
    FIELDWAY,
    COOMBE_LANE,
    GRAVEL_HILL,
    ADDINGTON_VILLAGE,
    NEW_ADDINGTON};

const uint16 LINE_TRAMS_B[] = {
    WIMBLEDON_A,
    DUNDONALD_ROAD,
    MERTON_PARK,
    WIMBLEDON_CHASE,
    SOUTH_MERTON,
    MORDEN_ROAD,
    PHIPPS_BRIDGE,
    BELGRAVE_WALK,
    MITCHAM,
    MITCHAM_JUNCTION_A,
    BEDDINGTON_LANE,
    AMPERE_WAY,
    THERAPIA_LANE,
    WADDON_MARSH,
    CENTRALE,
    WEST_CROYDON_A,
    WELLESLEY_ROAD,
    GEORGE_STREET,
    CHURCH_STREET,
    EAST_CROYDON_A,
    SANDILANDS,
    ARENA,
    AVENUE_ROAD,
    BIRKBECK,
    BECKENHAM_ROAD,
    BECKENHAM_JUNCTION};

const uint16 LINE_TRAMS_C[] = {
    WIMBLEDON_A,
    DUNDONALD_ROAD,
    MERTON_PARK,
    WIMBLEDON_CHASE,
    SOUTH_MERTON,
    MORDEN_ROAD,
    PHIPPS_BRIDGE,
    BELGRAVE_WALK,
    MITCHAM,
    MITCHAM_JUNCTION_A,
    BEDDINGTON_LANE,
    AMPERE_WAY,
    THERAPIA_LANE,
    WADDON_MARSH,
    CENTRALE,
    WEST_CROYDON_A,
    WELLESLEY_ROAD,
    GEORGE_STREET,
    CHURCH_STREET,
    EAST_CROYDON_A,
    SANDILANDS,
    BLACKHORSE_LANE,
    ADDISCOMBE,
    WOODSIDE,
    ELMERS_END};
