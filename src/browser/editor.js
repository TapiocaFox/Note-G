'use strict';

function saveByteArray(file_name, byte) {
  const blob = new Blob([byte], {
    type: "application/octet-stream"
  });
  const link = document.createElement('a');
  link.href = window.URL.createObjectURL(blob);
  link.download = file_name;
  link.click();
};

function NoteUnitInstructionLayer(settings) {
  this._time_unit_ms = settings.time_unit_ms;
  this._display = settings.display;
  this._screen_height_px;
  this._time_units_to_be_show = settings.time_units_to_be_show;
  this._time_units_passed = settings.time_units_passed;
  this._button_instruction_list = settings.button_instruction_list;
  this._display_dom;
}

NoteUnitInstructionLayer.prototype.initialize = function(animation_container_dom_object) {
  this._animation_container_dom_object = animation_container_dom_object;
  const animation_container_dom_object_style = window.getComputedStyle(this._animation_container_dom_object);
  const animation_container_dom_object_width = parseInt(animation_container_dom_object_style.getPropertyValue('width'));
  const animation_container_dom_object_height = parseInt(animation_container_dom_object_style.getPropertyValue('height'));
  this._screen_height_px = animation_container_dom_object_height;
  this._dom_object = document.createElement("div");
  this._dom_object.style.position = 'absolute';
  this._dom_object.style.height = '1px';
  this._dom_object.style.width = animation_container_dom_object_width + 'px';
  this._dom_object.style.background = 'linear-gradient(to right, #808080, #363636)';
  this._dom_object.style.top = this._time_units_passed * (this._screen_height_px / this._time_units_to_be_show) + 'px';
  const display = document.createElement("div");
  this._display_dom = display;

  display.style.position = 'absolute';
  display.style.left = '3px';
  display.style.bottom = '3px';
  display.style.color = 'white';
  display.style.opacity = '0.5';
  display.style.fontSize = '14px';
  display.innerHTML = this._display;
  if (this._display === 'rest') {
    this._dom_object.style.background = 'red';
    display.innerHTML = '';
  }
  this._dom_object.appendChild(display);
  if (this._button_instruction_list[0]) {
    const bar = document.createElement("div");
    bar.style.position = 'absolute';
    bar.style.height = '2px';
    bar.style.width = animation_container_dom_object_width / 4 + 'px';
    // bar.style.backgroundColor = '#1E90FF';
    bar.style.backgroundColor = 'white';
    bar.style.left = '0';
    bar.style.bottom = '0';
    this._dom_object.appendChild(bar);
  }
  if (this._button_instruction_list[1]) {
    const bar = document.createElement("div");
    bar.style.position = 'absolute';
    bar.style.height = '2px';
    bar.style.width = animation_container_dom_object_width / 4 + 'px';
    // bar.style.backgroundColor = '#FF0000';
    bar.style.backgroundColor = 'white';
    bar.style.left = animation_container_dom_object_width / 4 + 'px';
    bar.style.bottom = '0';
    this._dom_object.appendChild(bar);
  }
  if (this._button_instruction_list[2]) {
    const bar = document.createElement("div");
    bar.style.position = 'absolute';
    bar.style.height = '2px';
    bar.style.width = animation_container_dom_object_width / 4 + 'px';
    // bar.style.backgroundColor = '#00FF7F';
    bar.style.backgroundColor = 'white';
    bar.style.left = 2 * animation_container_dom_object_width / 4 + 'px';
    bar.style.bottom = '0';
    this._dom_object.appendChild(bar);
  }
  if (this._button_instruction_list[3]) {
    const bar = document.createElement("div");
    bar.style.position = 'absolute';
    bar.style.height = '2px';
    bar.style.width = animation_container_dom_object_width / 4 + 'px';
    // bar.style.backgroundColor = '#FFD700';
    bar.style.backgroundColor = 'white';
    bar.style.left = 3 * animation_container_dom_object_width / 4 + 'px';
    bar.style.bottom = '0';
    this._dom_object.appendChild(bar);
  }

  animation_container_dom_object.appendChild(this._dom_object);
};

NoteUnitInstructionLayer.prototype.nextLogicalTick = function(logical_ticks_interval_ms, global_logical_dynamic_parameters, remove_myself) {
  const time_units_passed_this_tick = (logical_ticks_interval_ms) / this._time_unit_ms;
  // console.log(time_units_passed_this_tick);
  this._time_units_passed += time_units_passed_this_tick;
  if (this._time_units_passed > this._time_units_to_be_show || global_logical_dynamic_parameters.remove) {
    remove_myself();
    this._dom_object.parentNode.removeChild(this._dom_object);
  }
};

NoteUnitInstructionLayer.prototype.updateGraphic = function(frames_per_second) {
  const opacity = 0.5 + 0.5 * (this._time_units_passed / this._time_units_to_be_show);
  this._display_dom.style.opacity = opacity;
  if (opacity > 0.95) {
    this._display_dom.style.color = '#ADFF2F';
  }
  this._dom_object.style.top = this._time_units_passed * (this._screen_height_px / this._time_units_to_be_show) + 'px';
};

function start() {
  let time_unit = 25;
  let timing_window = 25;
  let slower_multiplier = 2;

  let title = "Untitled";
  const time_units_to_be_show = 35;

  const audio_context = new(window.AudioContext || window.webkitAudioContext)();
  let note_unit_list = [{
    note_code_int: 37,
    tone_duration_time_units: 8,
    rest_duration_time_units: 1,
    button_instruction_list: [true, false, false, false],
    play_k_notes_int: 1,
    time_unit: time_unit,
    audio_context: audio_context
  }];

  const sheet_table = document.getElementById("sheet-table");
  const sheet_table_daf = new SimpleDOMAnimationFramework({
    animation_container_dom_object: sheet_table,
    logical_ticks_interval_ms: 200,
    frames_per_second: 3,
    global_logical_dynamic_parameters: {
      time_unit: time_unit
    },
    graphical_objects: [new NoteUnitRow(note_unit_list[0])],
    next_logical_tick: (global_logical_dynamic_parameters, create_graphical_object, update_global_logical_dynamic_parameters) => {
      update_global_logical_dynamic_parameters({
        time_unit: time_unit
      });
    }
  });

  const game_animation = document.getElementById("game-animation");
  const game_animation_daf = new SimpleDOMAnimationFramework({
    animation_container_dom_object: game_animation,
    logical_ticks_interval_ms: 20,
    frames_per_second: 50,
    global_logical_dynamic_parameters: {},
    graphical_objects: [],
    next_logical_tick: (global_logical_dynamic_parameters, create_graphical_object, update_global_logical_dynamic_parameters) => {
      update_global_logical_dynamic_parameters({
        time_unit: time_unit
      });
    }
  });
  sheet_table_daf.startAnimationAndLogicalTicking();



  const create_note_g_binary = () => {
    let data_bytes_uint8_list = [];

    // Header
    data_bytes_uint8_list.push(Math.round(time_unit));
    const note_unit_row_list = sheet_table.rows;
    const note_units_count = note_unit_row_list.length - 1;
    data_bytes_uint8_list.push(note_units_count & 255);
    data_bytes_uint8_list.push((note_units_count >> 8) & 255);
    data_bytes_uint8_list.push((note_units_count >> 16) & 255);
    data_bytes_uint8_list.push((note_units_count >> 24) & 255);
    data_bytes_uint8_list.push(timing_window);
    data_bytes_uint8_list.push(title.length);
    for (let i = 0; i < title.length; i++) {
      data_bytes_uint8_list.push(title.charCodeAt(i));
    }
    for (let i = 1; i <= note_units_count; i++) {
      const note_unit_row = note_unit_row_list[i].NoteUnitRow;
      // Note
      data_bytes_uint8_list.push(note_unit_row._note_code_int);
      // Tone duration
      data_bytes_uint8_list.push(note_unit_row._tone_duration_time_units);
      // Rest duration
      data_bytes_uint8_list.push(note_unit_row._rest_duration_time_units);
      // Instruction
      const button_instuction_4bit_int = (note_unit_row._button_instruction_list[0] ? 1 : 0) + 2 * (note_unit_row._button_instruction_list[1] ? 1 : 0) + 4 * (note_unit_row._button_instruction_list[2] ? 1 : 0) + 8 * (note_unit_row._button_instruction_list[3] ? 1 : 0);
      const play_k_notes_4bit_int = note_unit_row._play_k_notes_int;
      data_bytes_uint8_list.push((play_k_notes_4bit_int << 4) + button_instuction_4bit_int);
    }
    return new Uint8Array(data_bytes_uint8_list);
  };

  const decode_note_g_binary = (unit8_array) => {
    let pointer = 0;

    let result = {
      time_unit: null,
      note_units_count: null,
      timing_window: null,
      title: "",
      note_unit_list: []
    };

    // Header
    result.time_unit = unit8_array[pointer++];

    result.note_units_count = unit8_array[pointer++];
    result.note_units_count += unit8_array[pointer++] << 8;
    result.note_units_count += unit8_array[pointer++] << 16;
    result.note_units_count += unit8_array[pointer++] << 24;

    result.timing_window = unit8_array[pointer++];

    const title_length = unit8_array[pointer++];

    for (let i = 0; i < title_length; i++) {
      result.title += String.fromCharCode(unit8_array[pointer++]);
    }

    for (let i = 1; i <= result.note_units_count; i++) {
      const note_unit = {
        audio_context: audio_context
      };

      // Note
      note_unit.note_code_int = unit8_array[pointer++];
      // Tone duration
      note_unit.tone_duration_time_units = unit8_array[pointer++];
      // Rest duration
      note_unit.rest_duration_time_units = unit8_array[pointer++];

      // Instruction
      const instruction_8bit = unit8_array[pointer++];
      note_unit.button_instruction_list = [instruction_8bit & 1, instruction_8bit & 2, instruction_8bit & 4, instruction_8bit & 8];
      note_unit.play_k_notes_int = instruction_8bit >> 4 & 15;
      result.note_unit_list.push(note_unit);
    }
    return result;
  };

  const time_unit_text = document.getElementById("time-unit");
  const title_text = document.getElementById("title");
  const timing_window_text = document.getElementById("timing-window");
  time_unit_text.addEventListener('change', (event) => {
    time_unit = parseInt(event.target.value);
  });

  title_text.addEventListener('change', (event) => {
    title = event.target.value;
  });

  timing_window_text.addEventListener('change', (event) => {
    timing_window = parseInt(event.target.value);
  });


  let stop = true;
  let focused_note_unit_row = null;

  // Upload button
  function connectBLE() {
    const service = 0xFFE0;
    const characteristic = 0xFFE1;
    const packet_size = 16;
    let packet_num = 0;
    const ab = create_note_g_binary();
    const total_packet = Math.ceil(ab.length/packet_size);

    console.log(ab);
    //BLE setup. Connect and get service/characteristic notifications
    navigator.bluetooth.requestDevice({ filters: [{ services: [service] }] })
      .then(device => device.gatt.connect())
      .then(server => server.getPrimaryService(service))
      .then(service => service.getCharacteristic(characteristic))
      .then(characteristic => {
        function handleCharacteristicValueChanged(event) {
          if(packet_num < total_packet) {
            characteristic.writeValue(new Uint8Array([1, ...ab.slice(packet_num*packet_size, Math.min((packet_num+1)*packet_size), ab.length)]));
            packet_num++;
          }

          // const value = event.target.value;
          // console.log(value);
        }
        // const str = "123";
        // const arr = new Uint8Array(str.length);
        // for(var i=str.length; i--; )
        //     arr[i] = str.charCodeAt(i);
        characteristic.writeValue(new Uint8Array([0, total_packet]));
        return characteristic.startNotifications()
          .then(_ => {
            characteristic.addEventListener('characteristicvaluechanged',
              handleCharacteristicValueChanged);
          });
      })
      .then(_ => {
        console.log('Notifications have been started.');
      })
      .catch(error => {
        console.log(error);
      });


  }
  const upload_button = document.getElementById("upload-button");
  upload_button.addEventListener('click', () => {
    // console.log(123);
    connectBLE();
  });

  // Play button
  const play_button = document.getElementById("play-button");
  play_button.addEventListener('click', () => {
    stop = false;
    const note_unit_row_list = sheet_table.rows;

    let instruction_layer_index = 1;
    let time_unit_sum = 0;
    let note_unit_instruction_layer_list = [];
    while (time_unit_sum < time_units_to_be_show && instruction_layer_index < note_unit_row_list.length) {
      game_animation_daf.createGraphicalObject(new NoteUnitInstructionLayer({
        button_instruction_list: note_unit_row_list[instruction_layer_index].NoteUnitRow._button_instruction_list,
        time_unit_ms: time_unit,
        time_units_to_be_show: time_units_to_be_show,
        time_units_passed: time_units_to_be_show - time_unit_sum,
        display: Notes[note_unit_row_list[instruction_layer_index].NoteUnitRow._note_code_int][1]
      }));
      time_unit_sum += note_unit_row_list[instruction_layer_index].NoteUnitRow._tone_duration_time_units;
      time_unit_sum += note_unit_row_list[instruction_layer_index].NoteUnitRow._rest_duration_time_units;
      instruction_layer_index++;
    }

    const next_instruction_layer = (wait_time_units) => {
      setTimeout(() => {
        if (instruction_layer_index < note_unit_row_list.length && !stop) {
          let next_wait_time_units = 0;
          next_wait_time_units += note_unit_row_list[instruction_layer_index].NoteUnitRow._tone_duration_time_units;
          next_wait_time_units += note_unit_row_list[instruction_layer_index].NoteUnitRow._rest_duration_time_units;
          next_instruction_layer(next_wait_time_units);
          instruction_layer_index++;
          game_animation_daf.createGraphicalObject(new NoteUnitInstructionLayer({
            button_instruction_list: note_unit_row_list[instruction_layer_index - 1].NoteUnitRow._button_instruction_list,
            time_unit_ms: time_unit,
            display: Notes[note_unit_row_list[instruction_layer_index - 1].NoteUnitRow._note_code_int][1],
            time_units_to_be_show: time_units_to_be_show,
            time_units_passed: 0,
          }));
        } else {
          setTimeout(() => {
            game_animation_daf.pauseAnimationAndLogicalTicking();
          }, time_units_to_be_show * time_unit);
        }
      }, wait_time_units * time_unit);
    };
    next_instruction_layer((time_unit_sum - time_units_to_be_show));

    let index = 1;
    const next = () => {
      if (index < note_unit_row_list.length && !stop) {
        focused_note_unit_row = note_unit_row_list[index].NoteUnitRow;
        focused_note_unit_row.toneNote(time_unit, () => {
          index++;
          next();
        });
      }
    };
    game_animation_daf.startAnimationAndLogicalTicking();
    // game_animation_daf.pauseAnimationAndLogicalTicking();
    next();
  });

  // Play slower button
  const play_slower_button = document.getElementById("play-slower-button");
  play_slower_button.addEventListener('click', () => {
    stop = false;

    const note_unit_row_list = sheet_table.rows;

    let instruction_layer_index = 1;
    let time_unit_sum = 0;
    let note_unit_instruction_layer_list = [];
    while (time_unit_sum < time_units_to_be_show && instruction_layer_index < note_unit_row_list.length) {
      game_animation_daf.createGraphicalObject(new NoteUnitInstructionLayer({
        button_instruction_list: note_unit_row_list[instruction_layer_index].NoteUnitRow._button_instruction_list,
        time_unit_ms: time_unit * slower_multiplier,
        time_units_to_be_show: time_units_to_be_show,
        time_units_passed: time_units_to_be_show - time_unit_sum,
        display: Notes[note_unit_row_list[instruction_layer_index].NoteUnitRow._note_code_int][1]
      }));
      time_unit_sum += note_unit_row_list[instruction_layer_index].NoteUnitRow._tone_duration_time_units;
      time_unit_sum += note_unit_row_list[instruction_layer_index].NoteUnitRow._rest_duration_time_units;
      instruction_layer_index++;
    }

    const next_instruction_layer = (wait_time_units) => {
      setTimeout(() => {
        if (instruction_layer_index < note_unit_row_list.length && !stop) {
          let next_wait_time_units = 0;
          next_wait_time_units += note_unit_row_list[instruction_layer_index].NoteUnitRow._tone_duration_time_units;
          next_wait_time_units += note_unit_row_list[instruction_layer_index].NoteUnitRow._rest_duration_time_units;
          next_instruction_layer(next_wait_time_units);
          instruction_layer_index++;
          game_animation_daf.createGraphicalObject(new NoteUnitInstructionLayer({
            button_instruction_list: note_unit_row_list[instruction_layer_index - 1].NoteUnitRow._button_instruction_list,
            time_unit_ms: time_unit * slower_multiplier,
            display: Notes[note_unit_row_list[instruction_layer_index - 1].NoteUnitRow._note_code_int][1],
            time_units_to_be_show: time_units_to_be_show,
            time_units_passed: 0,
          }));

        } else {
          setTimeout(() => {
            game_animation_daf.pauseAnimationAndLogicalTicking();
          }, time_units_to_be_show * time_unit * slower_multiplier);
        }
      }, wait_time_units * time_unit * slower_multiplier);
    };
    next_instruction_layer((time_unit_sum - time_units_to_be_show));


    let index = 1;
    const next = () => {
      if (index < note_unit_row_list.length && !stop) {
        focused_note_unit_row = note_unit_row_list[index].NoteUnitRow;
        focused_note_unit_row.toneNote(time_unit * slower_multiplier, () => {
          index++;
          next();
        });

      }
    };
    game_animation_daf.startAnimationAndLogicalTicking();
    next();
  });

  // Stop
  const stop_button = document.getElementById("stop-button");
  stop_button.addEventListener('click', () => {
    stop = true;
  });

  document.addEventListener('keydown', (event) => {
    if (stop) {
      return;
    }
    // game.mergeRightward();
    // render();
    const keyName = event.key;
    if (keyName === 'z') {
      focused_note_unit_row.toggleButtonInstruction(0);
      return;
    } else if (keyName === 'x') {
      focused_note_unit_row.toggleButtonInstruction(1);
      return;
    } else if (keyName === 'c') {
      focused_note_unit_row.toggleButtonInstruction(2);
      return;
    } else if (keyName === 'v') {
      focused_note_unit_row.toggleButtonInstruction(3);
      return;
    }
  }, false);

  // Import button
  const import_button = document.getElementById("import-button");
  const import_file = document.getElementById("import-file");

  import_file.addEventListener('change', (event) => {
    const file = event.target.files[0];
    const splited = import_file.value.split(/(\\|\/|\.)/g);
    const file_extension = splited.pop();
    const reader = new FileReader();
    if (file_extension === 'xml') {

      const file_name = splited[splited.length - 2];
      title = file_name;
      title_text.value = file_name;
      reader.onload = (event) => {

        const new_note_unit_list = [];
        const text = event.target.result;
        const domparser = new DOMParser();
        const doc = domparser.parseFromString(text, 'text/xml');
        const divisions = parseInt(doc.getElementsByTagName("divisions")[0].innerHTML);
        const sound = doc.getElementsByTagName("sound")[0];
        const tempo = sound ? (sound.getAttribute('tempo') ? parseInt(sound.getAttribute('tempo')) : 120) : 120;
        const duration_unit = 1000.0 * 60.0 / tempo / divisions;

        const new_time_unit = duration_unit / 2;

        console.log('MusicXML imported');
        console.log('divisions: ', divisions);
        console.log('tempo: ', tempo);
        console.log('duration_unit: ', duration_unit);
        const measures = doc.getElementsByTagName("measure");
        let latest_default_x = 0;
        for (let index = 0; index < measures.length; index++) {
          const nodes = measures[index].childNodes;
          if (index === 0) {
            if (measures[index].getElementsByTagName("clef").length > 1) {
              alert('This sheet has more then 1 clef. This may cause some errors. Click ok to continue.');
            }
          }

          for (let j = 0; j < nodes.length; j++) {
            const item = nodes[j];
            if (item.tagName === 'note' || item.tagName === 'rest') {
              if (latest_default_x === item.getAttribute('default-x')) {
                continue;
              }
              latest_default_x = item.getAttribute('default-x');
              const step = item.getElementsByTagName('step')[0] ? Steps[item.getElementsByTagName('step')[0].innerHTML] : 0;
              const alter = item.getElementsByTagName('alter')[0] ? parseInt(item.getElementsByTagName('alter')[0].innerHTML) : 0;
              const octave = item.getElementsByTagName('octave')[0] ? parseInt(item.getElementsByTagName('octave')[0].innerHTML) : 0;
              let duration = item.getElementsByTagName('duration')[0] ? parseInt(item.getElementsByTagName('duration')[0].innerHTML) : 0;
              const quarters = item.getElementsByTagName('type')[0] ? TypeToQuarters[item.getElementsByTagName('type')[0].innerHTML] : 1;
              if (duration === 0) {
                new_time_unit
                duration = quarters * divisions;
              }
              // console.log('index: ', new_note_unit_list.length);
              // console.log('Note imported');
              // console.log('step: ', step);
              // console.log('alter: ', alter);
              // console.log('octave: ', octave);
              // console.log('duration: ', duration);
              // console.log('quarters: ', quarters);
              // console.log('quarters: ', item.getElementsByTagName('type')[0].innerHTML);
              new_note_unit_list.push({
                note_code_int: (item.getElementsByTagName('rest').length) ? 0 : 12 * octave + step + alter,
                tone_duration_time_units: duration * 2,
                rest_duration_time_units: 1,
                button_instruction_list: [false, false, false, false],
                play_k_notes_int: 1,
                time_unit: new_time_unit,
                audio_context: audio_context
              });
            } else if (item.tagName === 'backup') {
              break;
            } else {

            }
          }
        }
        time_unit = Math.round(new_time_unit);
        time_unit_text.value = time_unit;
        // console.log(new_note_unit_list);
        new_note_unit_list.forEach((settings, i) => {
          sheet_table_daf.createGraphicalObject(new NoteUnitRow(settings));
        });
      };
      reader.readAsText(file);
    } else if (file_extension === 'ngs') {
      reader.onload = (event) => {
        const ab = event.target.result;
        const sheet_data = decode_note_g_binary(new Uint8Array(ab));
        console.log(sheet_data);
        title = sheet_data.title;
        title_text.value = title;
        time_unit = sheet_data.time_unit;
        time_unit_text.value = time_unit;
        timing_window = sheet_data.timing_window;
        timing_window_text.value = timing_window;
        for (let i = 0; i < sheet_data.note_units_count; i++) {
          sheet_table_daf.createGraphicalObject(new NoteUnitRow(sheet_data.note_unit_list[i]));
        }
      };
      reader.readAsArrayBuffer(file);
    } else {
      alert('File must be either ".ngs" or ".xml" file.');
    }
  });

  import_button.addEventListener('click', () => {
    import_file.click();
  });

  const export_button = document.getElementById("export-button");

  export_button.addEventListener('click', () => {
    saveByteArray(title + '.ngs', create_note_g_binary());
  });
  // Clear button
  const clear_button = document.getElementById("clear-button");
  clear_button.addEventListener('click', () => {
    console.log(123);
    sheet_table_daf._graphical_objects.forEach((item, i) => {
      item._to_be_remove = true;
    });

  });
}

start();
