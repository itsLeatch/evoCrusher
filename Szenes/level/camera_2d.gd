extends Camera2D

var lastMousePos: Vector2 = Vector2(0,0)
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	lastMousePos = get_viewport().get_mouse_position()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if(Input.is_action_just_pressed("drag")):
		lastMousePos = get_viewport().get_mouse_position()
	if(Input.is_action_pressed("drag")):
		var newMousePos = get_viewport().get_mouse_position()
		position += (lastMousePos - newMousePos) / zoom
		lastMousePos = newMousePos
	if(Input.is_action_just_pressed("zoomOut")):
		zoom /= 1.1
	if(Input.is_action_just_pressed("zoomIn")):
		zoom *= 1.1
