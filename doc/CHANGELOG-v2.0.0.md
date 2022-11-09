# Breaking changes from all subversions of `v1`
- `--static-seed` is renamed to `--from-example-secret`
- JSON output is no longer formatted for humans. All whitespace is eliminated wherever possible. We suggest you use a utility like `jq` to view a nicely formatted JSON string.

# New features in `v2.0.0`

## Specifying

## Using `--seed N` to supply your own seed
- `--seed N` will allow you to specify your own 64 character hexadecimal string to use as the seed
```sh
./bls --seed 1122aaff1122aaff1122aaff1122aaff1122aaff1122aaff1122aaff1122aaff | jq
{
	"secret": "1667eed1016fe61ff80e07a03e91b911ec0286d1de2147bf47e2b6027fea11a3",
  "public": "06e17091e2c1aed3f261535142c5ece4cd246726b7706483fb34c6d50721f25f88d13fa947bb5e7f45d890e3bf1a0ac4"
}
```
- Seeds must be *exactly 64 hexadecimal characters*

## Generate a random seed
- Users can now ask the utility to generate a random seed
```sh
./bls -G | jq
{
  "seeds": [
    "2453a514049c8946c477cf410537848eeed9b8d6e07148683f2b434de1147b36"
  ]
}
```
or
```sh
./bls --generate-seed | jq
```
### Specifying how many random seeds to generate
- use the `--count` or `-c` flag to specify how many random seeds to generate:
```sh
./bls -G -c 3 | jq
{
  "seeds": [
    "7a58a1dbe40270c416845adc76d46b71144da235f024f60d5f9b2959a7ae0e3a",
    "248a659cf160c46e4f4e698a5dae7ac86ca6dc270ff62018d0cadab8ba654ed5",
    "959031cdc04ac5f43a3335f17dea5a241a1cae3e28dff418511655ccc083ee0a"
  ]
}
```
or
```sh
./bls --generate-seed -c 3 | jq
```
or
```sh
./bls --generate-seed --count 3 | jq
```

## Deriving child sk's
- Use `-i N` or `--index N` and the `Nth` index child key will be generated
```sh
./bls -i 2 | jq
{
  "parent": {
    "secret": "2b6d9c7dfac847f811af6c4f9b3a24321850fe758e5f029a884a070ca9fcf066",
    "public": "05b49619d46d7f9a53b58fd5f668225810cb9e7c8f463ed29e4457e90098c4d84430bf231670ba9ad7e2db64833bf4c2"
  },
  "children": [
    {
      "index": 2,
      "secret": "541402ce5af043b8fa411f52af96dc9f865bac6c6eec9020688c14e736f1e8e6",
      "public": "0885b8624d0e5f1f38392771785fc24e41b6abfd00c257a0c2597903fa9cc8f0c9be3346e2ca63146c7f506e762cad1d"
    }
  ]
}
```
- A CSV of integers may be specified for `N`:
```sh
./bls -i 1,2,3 | jq
{
  "parent": {
    "secret": "38098c1b3633c17ba0ee65d791c0ece754f16d149bc7d0ee8f2648a34e6f3517",
    "public": "1563122ac3a6e1e41c082195db41604d7b12c340445dc9c6532fd7b976522fc9bfa869638fdf6545f684157cf624b0d8"
  },
  "children": [
    {
      "index": 1,
      "secret": "1a91c725e1dd067b95b81bd0e19da51fe8fd5ab7da92ca9f33121fc3d2f542dd",
      "public": "992de20926cd0d24aa8f8b31866cbf43e1c0354d5efeeb5df4548e89672400817814b8b012527b1a3e53bbd7e3847b2e"
    },
    {
      "index": 2,
      "secret": "65fa1ff4cf9c0d0160fa1ada255093ad523258f808995f7e45688839277b81a6",
      "public": "817b36bcf64016c879abd378888abb65edb8bfccb86c001ec17fa3309b79ba19d56f838734227a915baa84c6aecb383d"
    },
    {
      "index": 3,
      "secret": "538ed044519bad162db54534387bd7c54aeb88d229014a06d0f87c78e3e4e363",
      "public": "90cfc3b3ace07aca8daf3ceab164a66bfab65f447f5d7a8b7f30b78b0ce3f8106421fd6240ff66cd5ff40128a50f6f85"
    }
  ]
}
```
- `--seed`, and `--from-secret` can be used with the `-i` flag:
```sh
./bls --seed 1122aaff1122aaff1122aaff1122aaff1122aaff1122aaff1122aaff1122aaff -i 2,4,5 | jq
{
  "parent": {
    "secret": "1667eed1016fe61ff80e07a03e91b911ec0286d1de2147bf47e2b6027fea11a3",
    "public": "06e17091e2c1aed3f261535142c5ece4cd246726b7706483fb34c6d50721f25f88d13fa947bb5e7f45d890e3bf1a0ac4"
  },
  "children": [
    {
      "index": 2,
      "secret": "1a3dcd20256e3a2bb089486341d829f3ce255132579c587d56e3b4fd19be70f3",
      "public": "942bbbbb29b9a1af59f4640fff2a6fe4b5126a66ec438c0caa929b24e559400a4b6179472a669f298784088c594fa387"
    },
    {
      "index": 4,
      "secret": "0cb2211e7021614af667ecbbb58e746ca74c10a8746b022b825b2154dbd2faa2",
      "public": "85d79861d8f5183312657ca24fb488c8369bbe915083a9eabe27ec656cb5ce5a3e2cca2f15746885aa730ed177137f48"
    },
    {
      "index": 5,
      "secret": "30969166d9a82db8fbb9afaa7a6563268939f2290a5826963e2453ddccdb22b1",
      "public": "1757fed9c34a1e7cc00b332fb2f5fa514fd399956c5e55f516e7d1c97d9a2cc8fc332d44e7088fe9ea5a2b10581c205d"
    }
  ]
}
```
