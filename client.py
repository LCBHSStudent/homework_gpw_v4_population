#!/bin/python3
import asyncio
import json
import ujson
from aiohttp import ClientSession
from argparse import ArgumentParser


single_polygon = '''{
	"type": "Feature",
	"properties": {},
	"geometry": {
		"type": "Polygon",
		"coordinates": [
            []
        ]
	}
}
'''


def create_signle_polygon_object(coordinates):
    '''create geojson only contains single polygon
    coordinates: list of list<float, 2>
    '''
    obj = json.loads(single_polygon)
    obj['geometry']['coordinates'][0].extend(coordinates)
    return obj


async def get_polygon_population(coordinates):
    async with ClientSession() as session:
        async with session.post(url='http://localhost:8000/api/get_polygon_population', json=create_signle_polygon_object(coordinates), headers={}) as resp:
            pass


if __name__ == '__main__':
    parser = ArgumentParser()
    # parser.add_argument('-b', '--begin', dest='begin', type=int, required=True)
    args = parser.parse_args()
    asyncio.run(get_polygon_population([
        [101.6455078125,27.68352808378776],
        [114.78515624999999,27.68352808378776],
        [114.78515624999999,35.209721645221386],
        [101.6455078125,35.209721645221386],
        [101.6455078125,27.68352808378776]
    ]))
    asyncio.run(get_polygon_population([
        [111.22558593749999,40.1452892956766],
        [106.69921875,37.92686760148135],
        [107.0068359375,36.491973470593685],
        [108.017578125,36.10237644873644],
        [107.22656249999999,35.38904996691167],
        [105.6005859375,35.38904996691167],
        [105.9521484375,32.65787573695528],
        [108.984375,31.42866311735861],
        [111.62109375,32.02670629333614],
        [111.4892578125,34.56085936708384],
        [111.0498046875,36.06686213257888],
        [111.62109375,39.740986355883564],
        [111.22558593749999,40.1452892956766]
    ]))
