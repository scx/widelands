dirname = path.dirname(__file__)

tribes:new_productionsite_type {
   msgctxt = "empire_building",
   name = "empire_hunters_house",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext("empire_building", "Hunter’s House"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "small",

   buildcost = {
      log = 1,
      planks = 1,
      granite = 1
   },
   return_on_dismantle = {
      granite = 1
   },

   animations = {
      idle = {
         pictures = path.list_files(dirname .. "idle_??.png"),
         hotspot = { 54, 55 },
      },
   },

   aihints = {
      collects_ware_from_map = "meat",
      prohibited_till = 400
   },

   working_positions = {
      empire_hunter = 1
   },

   indicate_workarea_overlaps = {
      empire_hunters_house = false,
   },

   programs = {
      work = {
         -- TRANSLATORS: Completed/Skipped/Did not start hunting because ...
         descname = _"hunting",
         actions = {
            "callworker=hunt",
            "sleep=35000"
         }
      },
   },
   out_of_resource_notification = {
      -- Translators: Short for "Out of Game" for a resource
      title = _"No Game",
      -- TRANSLATORS: "Game" means animals that you can hunt
      heading = _"Out of Game",
      -- TRANSLATORS: "game" means animals that you can hunt
      message = pgettext("empire_building", "The hunter working out of this hunter’s house can’t find any game in his work area."),
      productivity_threshold = 0
   },
}
