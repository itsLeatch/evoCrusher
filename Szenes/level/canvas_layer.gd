extends CanvasLayer


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	$Label.text = str($simulationSpeedSlider.value)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_simulation_speed_slider_drag_ended(value_changed: bool) -> void:
	$Label.text = str($simulationSpeedSlider.value)
