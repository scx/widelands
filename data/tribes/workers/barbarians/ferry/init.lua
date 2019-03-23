dirname = path.dirname (__file__)

-- TODO(Nordfriese): Make animations
animations = {
   idle = {
      pictures = path.list_files (dirname .. "idle_??.png"),
      hotspot = {29, 19},
   }
}
add_walking_animations (animations, "walk", dirname, "idle", {29, 19})
add_walking_animations (animations, "walkload", dirname, "idle", {29, 19})

tribes:new_ferry_type {
   msgctxt = "barbarians_worker",
   name = "barbarians_ferry",
   -- TRANSLATORS: This is a worker name used in lists of workers
   descname = pgettext ("barbarians_worker", "Ferry"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   vision_range = 2,

   animations = animations,
}
