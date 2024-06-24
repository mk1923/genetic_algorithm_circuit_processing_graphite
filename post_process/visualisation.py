import graphviz
from PIL import Image
import os


def read_vector_from_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        vector = [int(x.strip()) for x in lines[0].split(',')]
    return vector


def generate_graph_txt(vector, file_path):
    num_units = (len(vector) - 1) // 3

    # define nodes part
    nodes = ["NODES"]
    for i in range(num_units):
        nodes.append(f"{i} Unit_{i} box lightblue filled")
    nodes.append(f"{num_units} Concentrate box lightgreen filled")
    nodes.append(f"{num_units+1} Tailings box lightcoral filled")

    # define edges part
    edges = ["EDGES"]
    edges.append(f"Feed {vector[0]} black")

    for i in range(num_units):
        concentrate_dest = vector[1 + i * 3]
        intermediate_dest = vector[2 + i * 3]
        tailing_dest = vector[3 + i * 3]
        edges.append(f"{i} {concentrate_dest} concentrate blue")
        edges.append(f"{i} {intermediate_dest} intermediate purple")
        edges.append(f"{i} {tailing_dest} tailing red")

    # combine nodes and edges
    content = "\n".join(nodes) + "\n\n" + "\n".join(edges)

    # write to file
    with open(file_path, 'w') as file:
        file.write(content)


def read_graph_data(file_path):
    nodes = []
    edges = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        section = None
        for line in lines:
            line = line.strip()
            if not line:
                continue
            if line == 'NODES':
                section = 'nodes'
                continue
            elif line == 'EDGES':
                section = 'edges'
                continue

            if section == 'nodes':
                parts = line.split()
                node_id = parts[0]
                label = parts[1]
                shape = parts[2]
                color = parts[3]
                style = parts[4]
                nodes.append((node_id, label, shape, color, style))
            elif section == 'edges':
                parts = line.split()
                if len(parts) == 3:
                    start = parts[0]
                    end = parts[1]
                    label = None
                    color = parts[2]
                else:
                    start = parts[0]
                    end = parts[1]
                    label = parts[2]
                    color = parts[3]
                edges.append((start, end, label, color))
    return nodes, edges


def create_graph(nodes, edges, vector, file_name, show_labels=True):
    dot = graphviz.Digraph(comment='Graph from TXT')

    # set graph attributes
    dot.attr(rankdir='LR')
    dot.attr('node', shape='rectangle')
    dot.graph_attr.update(dpi='300')

    with dot.subgraph() as t:
        t.attr(rankdir='LR')
        t.attr('node', shape='rectangle')
        for node in nodes:
            node_id, label, shape, color, style = node
            t.node(node_id, label, shape=shape, color=color, style=style)

        for edge in edges:
            start, end, label, color = edge
            if label == 'concentrate':
                tailport = 'n'
                headport = 'w'
            elif label == 'intermediate':
                tailport = 'e'
                headport = 'w'
            elif label == 'tailing':
                tailport = 's'
                headport = 'w'
            else:
                tailport = 'e'
                headport = 'w'
            if show_labels:
                t.edge(start, end, label=label, color=color, tailport=tailport,
                       headport=headport, arrowhead='normal')
            else:
                t.edge(start, end, color=color, tailport=tailport,
                       headport=headport, arrowhead='normal')

    dot.render(file_name, format='png', cleanup=True)
    print(f'Graph saved to {file_name}.png')
    return dot


def create_vector_image(vector, file_name):
    num_units = (len(vector) - 1) // 3
    vector_label = '<<table border="0" cellborder="1" cellspacing="0"><tr>'
    vector_label += '<td>Feed</td>'
    for i in range(num_units):
        vector_label += f'<td colspan="3">Unit {i}</td>'
    vector_label += '</tr><tr>'
    for item in vector:
        vector_label += f'<td>{item}</td>'
    vector_label += '</tr></table>>'

    dot = graphviz.Digraph(comment='Vector Representation')
    dot.graph_attr.update(dpi='300')
    dot.node('vector', vector_label, shape='plaintext')
    dot.render(file_name, format='png', cleanup=True)
    print(f'Vector image saved to {file_name}.png')
    return dot


def merge_images(image1_path, image2_path, output_path):
    image1 = Image.open(image1_path)
    image2 = Image.open(image2_path)

    # Resize vector image to match the width of the graph image
    # Calculate the new height while maintaining aspect ratio
    aspect_ratio = image2.height / image2.width
    new_height = int(image1.width * aspect_ratio)
    image2 = image2.resize((image1.width, new_height),
                           Image.Resampling.LANCZOS)
    merged_height = image1.height + image2.height
    merged_image = Image.new('RGB', (image1.width, merged_height))

    merged_image.paste(image1, (0, 0))
    merged_image.paste(image2, (0, image1.height))

    merged_image.save(output_path)
    print(f'Merged image saved to {output_path}')

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    # vector = [0, 1, 3, 3, 2, 2, 0, 4, 1, 1, 1, 0, 5]
    vector_file_path = 'vector_data.txt'
    vector = read_vector_from_file(vector_file_path)

    file_path = 'graph_data.txt'

    generate_graph_txt(vector, file_path)

    with open(file_path, 'r') as file:
        print(file.read())

    nodes, edges = read_graph_data(file_path)

    graph_image_path = './output_img/graph_image'
    vector_image_path = './output_img/vector_image'

    create_graph(nodes, edges, vector, graph_image_path, show_labels=False)
    create_vector_image(vector, vector_image_path)

    graph_image_path += '.png'
    vector_image_path += '.png'
    output_image_path = 'merged_image.png'

    merge_images(graph_image_path, vector_image_path, output_image_path)


if __name__ == '__main__':
    main()
