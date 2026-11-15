package serialization

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestSerializeDeserializeTestify(t *testing.T) {
	original := createTestData()

	encoded, err := Serialize(original)
	require.NoError(t, err)
	require.NotEmpty(t, encoded)

	assert.Contains(t, string(encoded), `"version": 1`)

	restored, err := Deserialize(encoded)
	require.NoError(t, err)
	require.NotNil(t, restored)

	assert.Equal(t, original.Array.Get(), restored.Array.Get())
	assert.Equal(t, original.List.Get(), restored.List.Get())
	assert.Equal(t, original.DList.Get(), restored.DList.Get())
	assert.Equal(t, original.Queue.Get(), restored.Queue.Get())
	assert.Equal(t, original.Stack.Get(), restored.Stack.Get())

	assert.Equal(t, original.HashTable.Capacity(), restored.HashTable.Capacity())
	assert.Equal(t, original.HashTable.Entries(), restored.HashTable.Entries())

	assert.Equal(t, original.AVLTree.Inorder(), restored.AVLTree.Inorder())
}

func TestSerializeNilDataTestify(t *testing.T) {
	encoded, err := Serialize(nil)

	assert.Nil(t, encoded)
	assert.Error(t, err)
}

func TestSerializeIncompleteDataTestify(t *testing.T) {
	data := &Data{
		Array: nil,
	}

	encoded, err := Serialize(data)

	assert.Nil(t, encoded)
	assert.Error(t, err)
}

func TestDeserializeInvalidJSONTestify(t *testing.T) {
	restored, err := Deserialize([]byte(`{"version":`))

	assert.Nil(t, restored)
	assert.Error(t, err)
}

func TestDeserializeUnsupportedVersionTestify(t *testing.T) {
	input := []byte(`{"version":99,"hash_capacity":11}`)

	restored, err := Deserialize(input)

	assert.Nil(t, restored)
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "неподдерживаемая версия")
}

func TestDeserializeInvalidCapacityTestify(t *testing.T) {
	input := []byte(`{"version":1,"hash_capacity":0}`)

	restored, err := Deserialize(input)

	assert.Nil(t, restored)
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "вместимость")
}

func TestDeserializeUnknownFieldTestify(t *testing.T) {
	input := []byte(`{"version":1,"hash_capacity":11,"unknown":true}`)

	restored, err := Deserialize(input)

	assert.Nil(t, restored)
	assert.Error(t, err)
}

func TestDeserializeMultipleDocumentsTestify(t *testing.T) {
	input := []byte(`{"version":1,"hash_capacity":11} {"version":1}`)

	restored, err := Deserialize(input)

	assert.Nil(t, restored)
	assert.Error(t, err)
}

func TestDeserializeDuplicateHashKeysTestify(t *testing.T) {
	input := []byte(`{
		"version": 1,
		"hash_capacity": 11,
		"hash_entries": [
			{"key": 5, "value": "first"},
			{"key": 5, "value": "second"}
		]
	}`)

	restored, err := Deserialize(input)

	assert.Nil(t, restored)
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "повторяющийся ключ")
}
