dirname = path.dirname (__file__)

tribes:new_ferry_type {
   msgctxt = "barbarians_worker",
   name = "barbarians_ferry",
   -- TRANSLATORS: This is a worker name used in lists of workers
   descname = pgettext ("barbarians_worker", "Ferry"),
   helptext_script = dirname .. "helptexts.lua",
   animation_directory = dirname,
   icon = dirname .. "menu.png",
   vision_range = 2,

   -- TODO(Nordfriese): Make animations
   spritesheets = {
      idle = {
         hotspot = {29, 19},
         fps = 10,
         columns = 5,
         rows = 8,
         frames = 40,
      },
      walk = {
         basename = "sail",
         directional = true,
         hotspot = {29, 19},
         fps = 10,
         columns = 5,
         rows = 4,
         frames = 20,
      },
      walkload = {
         basename = "sail",
         directional = true,
         hotspot = {29, 19},
         fps = 10,
         columns = 5,
         rows = 4,
         frames = 20,
      }
   }
}
