/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2021 Antigloss Huang (https://github.com/antigloss) All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef LIBANT_SYSTEM_DAEMON_H_
#define LIBANT_SYSTEM_DAEMON_H_

namespace ant {

/**
 * Daemonize is for programs wishing to detach themselves from the controlling terminal and run in the background as system daemons.
 *
 * @param noChgDir set it true if you don't want to change the current working directory, or false if you want to change the cwd to /
 * @param noClose set it true if you don't want to close the standard file descriptors, or false if you want to redirect the standard input, output and error to /dev/null
 * @return true on success, false on failure.
 */
bool Daemonize(bool noChgDir, bool noClose);

} // namespace ant

#endif //LIBANT_SYSTEM_DAEMON_H_
