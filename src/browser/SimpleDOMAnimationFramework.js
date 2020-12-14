// 我自己寫的
// Code written by noowyee NCTU. 2020.11.15.

'use strict';

function SimpleDOMAnimationFramework(settings) {
  this._animation_container_dom_object = settings.animation_container_dom_object;

  this._global_logical_dynamic_parameters = settings.global_logical_dynamic_parameters;

  // Update rates.
  this._logical_ticks_interval_ms = settings.logical_ticks_interval_ms;
  this._frames_per_second = settings.frames_per_second;

  // Status
  this._animation_freezed = true;
  this._logical_ticking_freezed = true;

  this._next_logical_tick = settings.next_logical_tick;

  this._graphical_objects = settings.graphical_objects;

  this.initialize();
}

SimpleDOMAnimationFramework.prototype.initialize = function() {
  // Initialize graphical objects.
  this._graphical_objects.forEach((graphical_object, i) => {
    graphical_object.initialize(this._animation_container_dom_object, this);
  });
}

SimpleDOMAnimationFramework.prototype.createGraphicalObject = function(graphical_object) {
  graphical_object.initialize(this._animation_container_dom_object, this);
  this._graphical_objects.push(graphical_object);
}

SimpleDOMAnimationFramework.prototype.updateFramesPerSecond = function(new_frames_per_second) {
  this._frames_per_second = new_frames_per_second;
}

SimpleDOMAnimationFramework.prototype.nextLogicalTick = function()  {
  // Call next logical_tick first for global_logical_dynamic_parameters.
  const create_graphical_object = (graphical_object) => {
    this.createGraphicalObject(graphical_object);
  };
  const update_global_logical_dynamic_parameters = (new_global_logical_dynamic_parameters) => {
    this._global_logical_dynamic_parameters = new_global_logical_dynamic_parameters;
  };
  this._next_logical_tick(this._global_logical_dynamic_parameters, create_graphical_object, update_global_logical_dynamic_parameters);

  // Call each next logical_tick of graphical objects.
  let to_be_remove_graphical_objects_index_list = [];
  this._graphical_objects.forEach((graphical_object, i) => {
    const remove_myself = () => {
      to_be_remove_graphical_objects_index_list.push(i);
    };
    graphical_object.nextLogicalTick(this._logical_ticks_interval_ms, this._global_logical_dynamic_parameters, remove_myself);
  });

  // Destory graphical_objects.
  // Sort and reverse for properly remove item with correct index.
  to_be_remove_graphical_objects_index_list.sort().reverse().forEach((graphical_object_index) => {
    this._graphical_objects.splice(graphical_object_index, 1);
  });
}

SimpleDOMAnimationFramework.prototype.updateFrame = function()  {
  this._graphical_objects.forEach((graphical_object, i) => {
    graphical_object.updateGraphic();
  });
}

// ***** Animation *****

SimpleDOMAnimationFramework.prototype.startAnimation = function() {
  const next = () => {
    if(!this._animation_freezed) {
      this.updateFrame();
      const interval_ms = 1000/this._frames_per_second;
      setTimeout(next, interval_ms);
    }
  };
  this._animation_freezed = false;
  next();
}

SimpleDOMAnimationFramework.prototype.pauseAnimation = function() {
  this._animation_freezed = true;
}

// Alias.
SimpleDOMAnimationFramework.prototype.resumeAnimation = function() {
  if(this._animation_freezed) this.startAnimation();
}


// ***** LogicalTicking *****

SimpleDOMAnimationFramework.prototype.startLogicalTicking = function() {
  const next = () => {
    if(!this._logical_ticking_freezed) {
      this.nextLogicalTick();
      setTimeout(next, this._logical_ticks_interval_ms);
    }
  };
  this._logical_ticking_freezed = false;
  next();
}

SimpleDOMAnimationFramework.prototype.pauseLogicalTicking = function() {
  this._logical_ticking_freezed = true;
}

// Alias.
SimpleDOMAnimationFramework.prototype.resumeLogicalTicking = function() {
  if(this._logical_ticking_freezed) this.startLogicalTicking();
}

// ***** Animation and LogicalTicking *****

SimpleDOMAnimationFramework.prototype.startAnimationAndLogicalTicking = function() {
  this.startLogicalTicking();
  this.startAnimation();
}

SimpleDOMAnimationFramework.prototype.pauseAnimationAndLogicalTicking = function() {
  this.pauseLogicalTicking();
  this.pauseAnimation();
}

SimpleDOMAnimationFramework.prototype.resumeAnimationAndLogicalTicking = function() {
  this.resumeLogicalTicking();
  this.resumeAnimation();
}
