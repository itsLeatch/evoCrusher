extends CharacterBody2D

@export var speed :float = 300
var weights :Float32Matrix = Float32Matrix.new(Vector2i(2,2))
var moveDirection :Float32Matrix =  Float32Matrix.new(Vector2i(1,2))
var energy: float = 0

# Called when the node enters the scene tree for the first time.
var rng = RandomNumberGenerator.new()
func _ready() -> void:
	moveDirection.data = [1,1]
	var randomWeightData = PackedFloat32Array()
	print(weights.rect())
	for i in range(weights.x * weights.y):
		randomWeightData.append(rng.randf_range(-1,1))
	weights.data = randomWeightData
	#print(weights.data)
	global_position = Vector2(rng.randf_range(-Global.mapSize.x / 2,Global.mapSize.x / 2), rng.randf_range(-Global.mapSize.y / 2,Global.mapSize.y / 2))
 	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	var result = weights.mul(moveDirection)
	var resVelocity = Vector2(result.get_value(Vector2(0,0)), result.get_value((Vector2(0,1))))
	#normalize
	resVelocity /= resVelocity.length()
	velocity = resVelocity * speed
	$BotImage.rotate(1*delta)
	move_and_slide()
	$Label.text = str(energy)

func incrementFood() ->void:
	energy+=1
