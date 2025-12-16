extends CharacterBody2D

@export var speed :float = 300

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	velocity = Input.get_vector("left", "right", "top", "bottom") * speed	
	$BotImage.rotate(1*delta)
	move_and_slide()
	
