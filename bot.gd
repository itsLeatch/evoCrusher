extends CharacterBody2D

@export var speed :float = 300
var moveDirection :Vector2 = Vector2(0,0)

# Called when the node enters the scene tree for the first time.
var rng = RandomNumberGenerator.new()
func _ready() -> void:
	moveDirection = Vector2(rng.randf_range(-1,1), rng.randf_range(-1,1))
	moveDirection /= moveDirection.length()
	
	global_position = Vector2(rng.randf_range(-Global.mapSize.x / 2,Global.mapSize.x / 2), rng.randf_range(-Global.mapSize.y / 2,Global.mapSize.y / 2))
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	velocity = moveDirection * speed
	$BotImage.rotate(1*delta)
	move_and_slide()
