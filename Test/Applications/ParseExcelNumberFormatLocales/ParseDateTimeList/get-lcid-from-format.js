/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

'use strict';
const { readFile, writeFile } = require("node:fs/promises");

async function startTest() {
  let args = process.argv.slice(2);
  if (args.length < 2) {
    console.error(`missing arguments.USAGE: ${process.argv[0]} "inCsv" "outJson"`);
    return;
  }
  console.info("test started");

  let missing = [];
  let duplicates = [];
  let unique = {};
  let res = [];
  res.push('{');
  let text = await readFile(args[0], {encoding: 'utf8'});
  let lines = text.split('\n');
  for (let i = 0; i < lines.length; ++i) {
    let elem = lines[i];
    elem = elem.replace(/[\r\,]*$/, "");
    const found = elem.match(/\[\$-([A-F0-9]*)\]/g);
    if (found) {
      let LCID = 0;
      for (let j = 0; j < found.length; ++j) {
        let test = parseInt(found[j].substring(3), 16) & 0xFFFF;
        if (test > 0) {
          LCID = test;
          if(!unique[LCID]) {
            break;
          }
        }
      }
      // console.log(LCID);
      if(LCID > 0) {
        if(!unique[LCID]) {
          unique[LCID] = 1;
          res.push(`${LCID}:[${elem}],`);
        } else {
          duplicates.push(i);
        }
      } else {
        missing.push(i);
      }
    } else {
      missing.push(i);
    }
  }
  res.push('}');
  res.push('missing:' + JSON.stringify(missing));
  res.push('duplicates:' + JSON.stringify(duplicates));

  await writeFile(args[1], res.join('\n'), {encoding: 'utf8'});

  console.info("test finished");
}

startTest().then(()=>{
  //delay to log observer events
}).catch((err) => {
  console.error(err.stack);
}).finally(() => {
  process.exit(0);
});
