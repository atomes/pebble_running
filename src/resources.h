#define INTERVAL_TYPE_RUN 0
#define INTERVAL_TYPE_WALK 1
#define INTERVAL_TYPE_PERIODIC 2
#define INTERVAL_TYPE_WARMUP 3
#define INTERVAL_TYPE_COOLDOWN 4
#define MAX_MENU_ITEMS 17
#define TIMER_FREQUENCY_MS 1000

// TODO: Should we define text as constants too?
#define MESSAGE_COMPLETED "Done!"
#define MESSAGE_WALK "Walk"
#define MESSAGE_RUN "Run"
#define MESSAGE_WARMUP "Warm"
#define MESSAGE_COOLDOWN "Cool"
#define MESSAGE_PERIOD "period %d of %d"


// Interval consists of type and duration.
struct interval {
    int type;
    int duration;
};

// Control block is a control structure used
// by the Running Coach to loop through the
// running program.
struct control_block {
    // Should the first interval be looped? Used for the interval program.
    bool loop_first;

    // Pointer to the current interval.
    struct interval *current_interval;

    // Index of the current interval within the program.
    int current_interval_index;
    
    // Seconds left in the current interval.
    int current_interval_seconds_left;
    
    // Intervals left in the program.
    int intervals_left;

    // Total number intervals in the program.
    int intervals_total;

    // Has we finished an interval.
    bool is_interval_over;

    // Have we finished a program.
    bool is_program_over;

    // Title of the currently selected program.
    char *program_title;
};

struct menu_item {
    // Title.
    char title[10];

    // Subtitle.
    char subtitle[20];

    // Number of intervals.
    int intervals_cnt;

    // Intervals array.
    struct interval intervals[20];

    // If this instance is the main menu, this is the link to the program menu.
    struct menu_item *program_menu;

    // Number of items in the program menu.
    int program_menu_size;
};

struct menu_item Test_menu[] = {
    {
        .title = "Week 1",       .subtitle = "Days 1-3",
        .intervals_cnt = 3,
        .intervals = {            {.type= INTERVAL_TYPE_WARMUP,      .duration = 2},
            {.type = INTERVAL_TYPE_RUN,       .duration = 3},
            {.type = INTERVAL_TYPE_WALK,      .duration = 5}
        }
    }
};

struct menu_item F25K_menu[] = {
    {
        .title = "Week 1",       .subtitle = "Days 1-3",
        .intervals_cnt = 17,
        .intervals = {
            {.type = INTERVAL_TYPE_WARMUP,    .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 60},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 60},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 60},
            {.type = INTERVAL_TYPE_WALK,      .duration = 95},
            {.type = INTERVAL_TYPE_RUN,       .duration = 65},
            {.type = INTERVAL_TYPE_WALK,      .duration = 95},
            {.type = INTERVAL_TYPE_RUN,       .duration = 65},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 65},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 60},
            {.type = INTERVAL_TYPE_WALK,      .duration = 95},
            {.type = INTERVAL_TYPE_RUN,       .duration = 65},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 180}
        }
    },
    {
        .title = "Week 2",       .subtitle = "Days 1-3",
        .intervals_cnt = 13,
        .intervals = {
            {.type = INTERVAL_TYPE_WARMUP,    .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_WALK,      .duration = 120},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_WALK,      .duration = 120},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_WALK,      .duration = 120},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_WALK,      .duration = 120},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_WALK,      .duration = 120},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 110}
        }
    },
    {
        .title = "Week 3",       .subtitle = "Days 1-3",
        .intervals_cnt = 9,
        .intervals = {
            {.type = INTERVAL_TYPE_WARMUP,    .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 180},
            {.type = INTERVAL_TYPE_WALK,      .duration = 180},
            {.type = INTERVAL_TYPE_RUN,       .duration = 90},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 180},             
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 250}
        }
    },
    {
        .title = "Week 4",       .subtitle = "Days 1-3",
        .intervals_cnt = 9,
        .intervals = {
            {.type = INTERVAL_TYPE_WARMUP,    .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 180},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 300},
            {.type = INTERVAL_TYPE_WALK,      .duration = 150},
            {.type = INTERVAL_TYPE_RUN,       .duration = 180},
            {.type = INTERVAL_TYPE_WALK,      .duration = 90},
            {.type = INTERVAL_TYPE_RUN,       .duration = 300},            
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 120}
        }
    },  
    {
        .title = "Week 5",       .subtitle = "Day 1",
        .intervals_cnt = 7,
        .intervals = {
            {.type = INTERVAL_TYPE_WARMUP,    .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 300},
            {.type = INTERVAL_TYPE_WALK,      .duration = 180},
            {.type = INTERVAL_TYPE_RUN,       .duration = 300},
            {.type = INTERVAL_TYPE_WALK,      .duration = 180},
            {.type = INTERVAL_TYPE_RUN,       .duration = 300},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 250}
        }
    },    
    {
        .title = "Week 5",       .subtitle = "Day 2",
        .intervals_cnt = 5,
        .intervals = {            
		    {.type= INTERVAL_TYPE_WARMUP,     .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 480},
            {.type = INTERVAL_TYPE_WALK,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 480},
            {.type = INTERVAL_TYPE_COOLDOWN,      .duration = 105}
        }
    },
    {
        .title = "Week 5",       .subtitle = "Day 3",
        .intervals_cnt = 3,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1200},
            {.type = INTERVAL_TYPE_COOLDOWN,      .duration = 180}
        }
    },
    {
        .title = "Week 6",       .subtitle = "Day 1",
        .intervals_cnt = 7,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,     .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 300},
            {.type = INTERVAL_TYPE_WALK,      .duration = 180},
            {.type = INTERVAL_TYPE_RUN,       .duration = 480},
            {.type = INTERVAL_TYPE_WALK,      .duration = 180},
            {.type = INTERVAL_TYPE_RUN,       .duration = 300},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 240}
        }
    },
    {
        .title = "Week 6",       .subtitle = "Day 2",
        .intervals_cnt = 7,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,     .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 600},
            {.type = INTERVAL_TYPE_WALK,      .duration = 180},
            {.type = INTERVAL_TYPE_RUN,       .duration = 600},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 240}
        }
    },      
    {
        .title = "Week 6",       .subtitle = "Day 3",
        .intervals_cnt = 3,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,     .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1500},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 210}
        }
    },      
    {
        .title = "Week 7",       .subtitle = "Days 1-3",
        .intervals_cnt = 3,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,     .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1500},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 240}
        }
    },            
    {
        .title = "Week 8",       .subtitle = "Days 1-3",
        .intervals_cnt = 3,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,     .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1680},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 270}
        }
    },       
    {
        .title = "Week 9",       .subtitle = "Days 1-3",
        .intervals_cnt = 3,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,     .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1800},
            {.type = INTERVAL_TYPE_COOLDOWN,  .duration = 270}
        }
    },
    {
      .title = "Week 10",       .subtitle = "Days 1-3",
      .intervals_cnt = 3,
      .intervals = {
        {.type = INTERVAL_TYPE_WALK,      .duration = 300},
        {.type = INTERVAL_TYPE_RUN,       .duration = 2100},
        {.type = INTERVAL_TYPE_COOLDOWN,      .duration = 270}
      }
    }
};

struct menu_item F210K_menu[] = {
    {
        .title = "Week 1",       .subtitle = "Days 1-3",
        .intervals_cnt = 9,
        .intervals = {            
		    {.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 600},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 600},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 600},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 600},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    },
    {
        .title = "Week 2",       .subtitle = "Days 1-3",
        .intervals_cnt = 7,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 900},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 900},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 900},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    },  
    {
        .title = "Week 3",       .subtitle = "Days 1-3",
        .intervals_cnt = 7,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1020},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1020},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1020},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    },  
    {
        .title = "Week 4",       .subtitle = "Days 1-3",
        .intervals_cnt = 7,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1080},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1080},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1080},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    },
    {
        .title = "Week 5",       .subtitle = "Day 1",
        .intervals_cnt = 5,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1320},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1320},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    },      
    {
        .title = "Week 5",       .subtitle = "Day 2",
        .intervals_cnt = 5,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1500},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1500},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    },      
    {
        .title = "Week 5",       .subtitle = "Day 3",
        .intervals_cnt = 5,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1800},
            {.type = INTERVAL_TYPE_WALK,      .duration = 60},
            {.type = INTERVAL_TYPE_RUN,       .duration = 1800},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    },      
    {
        .title = "Week 6",       .subtitle = "Day 1-3",
        .intervals_cnt = 3,
        .intervals = {            
			{.type= INTERVAL_TYPE_WARMUP,      .duration = 300},
            {.type = INTERVAL_TYPE_RUN,       .duration = 3600},
            {.type = INTERVAL_TYPE_COOLDOWN,       .duration=300} 
		}        
    }
};

struct menu_item interval_menu[] = {
    {
        .title = "05 sec",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 05}
        }
    },
    {
        .title = "10 sec",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 10}
        }
    },
    {
        .title = "20 sec",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 20}
        }
    },
    {
        .title = "30 sec",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 30}
        }
    },
    {
        .title = "40 sec",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 40}
        }
    },
    {
        .title = "50 sec",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 50}
        }
    },
    {
        .title = "1 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 60}
        }
    },       
    {
        .title = "2 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 120}
        }
    }, 
    {
        .title = "3 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 180}
        }
    },                     
    {
        .title = "4 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 240}
        }
    },       
    {
        .title = "5 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 300}
        }
    }, 
    {
        .title = "10 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 600}
        }
    }, 
    {
        .title = "15 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 900}
        }
    }, 
    {
        .title = "20 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 1200}
        }
    }, 
    {
        .title = "25 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 1500}
        }
    },     
    {
        .title = "30 min",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 1800}
        }
    },
    {
        .title = "1 hour",       .subtitle = "",
        .intervals_cnt = 1,
        .intervals = {
            {.type = INTERVAL_TYPE_PERIODIC,      .duration = 3600}
        }
    }
};

struct menu_item main_menu[] = {
    //{.title = "Test",       .subtitle = "Test menu",            .program_menu = Test_menu,      .program_menu_size = 1},
    {.title = "F25K",       .subtitle = "First day to 5K",      .program_menu = F25K_menu,      .program_menu_size = 15},
//     {.title = "G28k",       .subtitle = "Gateway to 8k",        .program_menu = G28K_menu,     .program_menu_size = 8},
    {.title = "F210K",      .subtitle = "Freeway to 10K",       .program_menu = F210K_menu,     .program_menu_size = 8},
    {.title = "Intervals",  .subtitle = "Periodic Vibration",   .program_menu = interval_menu,  .program_menu_size = 17}
};