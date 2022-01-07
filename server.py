__mode__ = 'prod' # prod

# create utils
def flip_coords(polygon):
    return [[p[0], -1 * p[1]] for p in polygon]

def move_coords(polygon):
    return [[p[0] + 180, p[1] + 90] for p in polygon]


# process dataset in prod mode
if __mode__ == 'prod':
    import numpy as np

    skip_length = 64802 # all -9999, in fact is 64801
    shape = (10800, 10800)
    indices = np.zeros((8, shape[0]), dtype=int, order='C')

    for file_index in range(1, 9):
        with open(f'gpw_v4_population_count_rev11_2020_30_sec_{file_index}.asc', 'r') as file:
            for line_index in range(0, 6):
                file.readline()
            for line_index in range(0, shape[0]):
                indices[file_index - 1][line_index] = file.tell()
                file.readline()


# init Sanic app
import libgeo
import numpy as np
from sanic import Sanic
from sanic.response import json

app = Sanic('PRFApplication')
@app.post('/api/get_polygon_population')
async def polygon_query(request):
    polygon = request.json['geometry']['coordinates'][0]
    tile_info = libgeo.query_included_tiles(move_coords(flip_coords(polygon)), 0.008333333333333333)
    
    file_index = 1
    
    result = [[] for _ in range(0, 8)]
    
    for each_block in tile_info:
        if len(each_block) == 0:
            file_index += 1
            continue
        with open(f'gpw_v4_population_count_rev11_2020_30_sec_{file_index}.asc', 'r') as file:
            for row in each_block:
                file.seek(indices[file_index - 1][row[0]])
                line = file.readline()
                population_data = np.fromstring(line, dtype=float, count=row[1][-1][1] + 1)
                result.append([row[0], row[1], [population_data[r[0]:r[1] + 1] for r in row[1]]])
        file_index += 1
    
    return json(body={
        'res': result
    }, status=200)
