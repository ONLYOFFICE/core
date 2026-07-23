/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

'use strict';
const { readFile, writeFile } = require("node:fs/promises");

async function startTest() {
  let args = process.argv.slice(2);
  if (args.length < 3) {
    console.error(`missing arguments.USAGE: ${process.argv[0]} "in-time.csv" "in-date.csv" "out.json"`);
    return;
  }
  console.info("test started");

  let missing = [];
  let duplicates = {};
  let unique = {};

  let outLineMap = {};
  let outDates = {};
  let outTimes = {};
  let textDate = await readFile(args[0], {encoding: 'utf8'});
  let linesDate = textDate.split('\n');
  let textTime = await readFile(args[1], {encoding: 'utf8'});
  let linesTime = textTime.split('\n');
  for (let i = 0; i < linesDate.length && i < linesTime.length; ++i) {
    // console.log(i)
    outLineMap[i] = 0;
    let lineDate = linesDate[i];
    lineDate = lineDate.replace(/[\r\,]*$/, "");
    lineDate = lineDate.replace(/\\/g, '\\\\');
    lineDate = lineDate.replace(/\"\"\"/g, '"\\"');
    lineDate = lineDate.replace(/\"\"/g, '\\"');
    let lineTime = linesTime[i];
    lineTime = lineTime.replace(/[\r\,]*$/, "");
    lineTime = lineTime.replace(/\\/g, '\\\\');
    lineTime = lineTime.replace(/\"\"\"/g, '"\\"');
    lineTime = lineTime.replace(/\"\"/g, '\\"');

    let found = lineDate.match(/\[\$-([A-F0-9]*)\]/g);
    if (!found) {
      found = lineTime.match(/\[\$-([A-F0-9]*)\]/g);
    } else {
      let foundTime = lineTime.match(/\[\$-([A-F0-9]*)\]/g);
      if (foundTime) {
        found = found.concat(foundTime);
      }
    }
    if (found) {
      let LCID = 0;
      let LCIDOrigin = 0;
      for (let j = 0; j < found.length; ++j) {
        let test = parseInt(found[j].substring(3), 16) & 0xFFFF;
        if (test > 0) {
          LCID = test;
          LCIDOrigin = found[j]
          if(!unique[LCID]) {
            break;
          }
        }
      }
      // console.log(LCID);
      if(LCID > 0) {
        if(!unique[LCID]) {
          unique[LCID] = 1;
          outLineMap[i] = LCID;
          outDates[LCID] = JSON.parse(`[${lineDate}]`);
          outTimes[LCID] = JSON.parse(`[${lineTime}]`);
        } else {
          duplicates[i] = LCID + "-" + LCIDOrigin;
        }
      } else {
        missing.push(i);
      }
    } else {
      missing.push(i);
    }
  }
  let res = [];
  res.push(JSON.stringify(outLineMap, null, 2));
  res.push(JSON.stringify(outDates, null, 2));
  res.push(JSON.stringify(outTimes, null, 2));
  res.push('missing:' + JSON.stringify(missing));
  res.push('duplicate_line_to_lcid:' + JSON.stringify(duplicates, null, 2));

  await writeFile(args[2], res.join('\n'), {encoding: 'utf8'});

  console.info("test finished");
}

startTest().then(()=>{
  //delay to log observer events
}).catch((err) => {
  console.error(err.stack);
}).finally(() => {
  process.exit(0);
});
