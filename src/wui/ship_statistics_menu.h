/*
 * Copyright (C) 2017 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef WL_WUI_SHIP_STATISTICS_MENU_H
#define WL_WUI_SHIP_STATISTICS_MENU_H

#include <unordered_map>

#include "base/i18n.h"
#include "logic/map_objects/tribes/ship.h"
#include "notifications/notifications.h"
#include "ui_basic/button.h"
#include "ui_basic/table.h"
#include "ui_basic/unique_window.h"

class InteractivePlayer;

///  Shows a list of the ships owned by the interactive player.
struct ShipStatisticsMenu : public UI::UniqueWindow {
	ShipStatisticsMenu(InteractivePlayer&, UI::UniqueWindow::Registry&);

	bool handle_key(bool down, SDL_Keysym code) override;

private:
	enum Cols { ColName, ColStatus };
	enum class ShipFilterStatus {
		kIdle,
		kShipping,
		kExpeditionWaiting,
		kExpeditionScouting,
		kExpeditionPortspaceFound,
		kExpeditionColonizing,
		kAll
	};
	const std::string status_to_string(ShipFilterStatus status) const;
	const Image* status_to_image(ShipFilterStatus status) const;

	struct ShipInfo {
		ShipInfo(const std::string& init_name,
		         const ShipFilterStatus init_status,
		         const Widelands::Serial init_serial)
		   : name(init_name), status(init_status), serial(init_serial) {
		}
		ShipInfo() : ShipInfo("", ShipFilterStatus::kAll, 0) {
		}
		ShipInfo(const ShipInfo& other) : ShipInfo(other.name, other.status, other.serial) {
		}
		bool operator==(const ShipInfo& other) const {
			return serial == other.serial;
		}
		bool operator<(const ShipInfo& other) const {
			return serial < other.serial;
		}

		const std::string name;
		ShipFilterStatus status;
		const Widelands::Serial serial;
	};

	const ShipInfo* create_shipinfo(const Widelands::Ship& ship) const;
	Widelands::Ship* serial_to_ship(Widelands::Serial serial) const;

	InteractivePlayer& iplayer() const;
	void selected();
	void double_clicked();
	void set_buttons_enabled();

	void fill_table();
	void center_view();
	void watch_ship();
	void open_ship_window();
	/// Updates the status for the ship. If the ship is new, adds it to the table.
	void update_ship(const Widelands::Ship&);
	void remove_ship(Widelands::Serial serial);
	void set_entry_record(UI::Table<uintptr_t>::EntryRecord*, const ShipInfo& info);
	void update_entry_record(UI::Table<uintptr_t>::EntryRecord& er, const ShipInfo&);

	void filter_ships(ShipFilterStatus status);
	void toggle_filter_ships_button(UI::Button&, ShipFilterStatus);
	void set_filter_ships_tooltips();

	// Buttons for ship states
	UI::Button* waiting_btn_;
	UI::Button* scouting_btn_;
	UI::Button* portspace_btn_;
	UI::Button* colonizing_btn_;
	UI::Button* shipping_btn_;
	UI::Button* idle_btn_;
	ShipFilterStatus ship_filter_;
	// Navigation buttons
	UI::Button* watchbtn_;
	UI::Button* openwindowbtn_;
	UI::Button* centerviewbtn_;

	// Data
	UI::Table<uintptr_t> table_;
	std::unordered_map<Widelands::Serial, const ShipInfo*> data_;

	std::unique_ptr<Notifications::Subscriber<Widelands::NoteShipWindow>>
	   shipwindownotes_subscriber_;
	std::unique_ptr<Notifications::Subscriber<Widelands::NoteShipMessage>> shipnotes_subscriber_;
};

#endif  // end of include guard: WL_WUI_SHIP_STATISTICS_MENU_H
