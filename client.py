#!/bin/python3
import asyncio
import json
import numpy as np
import matplotlib.pyplot as plt
import mpl_toolkits.axisartist as axisartist
from matplotlib.colors import LogNorm
from aiohttp import ClientSession
from argparse import ArgumentParser


grid_size = 0.008333333333333333
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
    geojson = create_signle_polygon_object(coordinates)
    
    # req = gpd.read_file(geojson)
    
    async with ClientSession() as session:
        async with session.post(url='http://localhost:8000/api/get_polygon_population', json=geojson, headers={}) as resp:
            if resp.status == 200:
                result = json.loads(await resp.text())

                fig = plt.figure(1, (4, 8), dpi=450)
                ax = plt.gca()
                ax.spines['right'].set_color('none')
                ax.spines['top'].set_color('none')
                ax.xaxis.set_ticks_position('bottom')
                ax.yaxis.set_ticks_position('left')
                ax.spines['bottom'].set_position(('data', 0))
                ax.spines['left'].set_position(('data', 0))

                # plt.xlim(-180, 180)
                # plt.ylim(-90, 90)

                x = []
                y = []
                z = []
                z_total = 0
                for index in range(0, len(result['res'])):
                    block = result['res'][index]
                    base_lat = int(index % 4) * 90 - 180
                    base_lon = -int(index / 4) * 90 + 90
                    # [line_number, [start_idx, end_idx], [data......]]
                    for line in block:
                        lon = base_lon - grid_size * line[0]
                        # simple polygon
                        lat = base_lat + grid_size * line[1][0][0]
                        dist = 0
                        for data in line[2][0]:
                            x.append(lat + dist * grid_size)
                            y.append(lon)
                            z.append(data if data != -9999 else 0)
                            z_total += data
                            dist += 1
                
                plt.scatter(x, y, s=grid_size * 8, c=z, cmap='Spectral_r', linewidths=grid_size * 8)
                plt.clim(0, z_total / len(z) * 2)
                plt.colorbar()

                # draw polygon
                pts = len(coordinates)
                for index in range(0, pts + 1):
                    pt1 = coordinates[index % pts]
                    pt2 = coordinates[(index + 1) % pts]
                    plt.plot([pt1[0], pt2[0]], [pt1[1], pt2[1]], color='black', linewidth=1)

                fig.savefig('result.png')
                pass


if __name__ == '__main__':
    parser = ArgumentParser()
    # parser.add_argument('-b', '--begin', dest='begin', type=int, required=True)
    args = parser.parse_args()
    # asyncio.run(get_polygon_population([
    #     [101.6455078125,27.68352808378776],
    #     [114.78515624999999,27.68352808378776],
    #     [114.78515624999999,35.209721645221386],
    #     [101.6455078125,35.209721645221386],
    #     [101.6455078125,27.68352808378776]
    # ]))
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
