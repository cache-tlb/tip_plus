#include "GLMesh.h"
#include "VSShaderLibQT.h"

GLMesh::GLMesh(QOpenGLFunctionsType *env):
    has_coords(true), has_normals(false),
    context(env),
    vao(-1), vertex_coord_buff(-1), texture_coord_buff(-1), normal_buff(-1), tri_index_buff(-1)
{
    // TODO
}

void GLMesh::clear() {
    vertices.clear();
    texture_coords.clear();
    normals.clear();
    triangles.clear();
}

void GLMesh::add_vertex(const Vec3f &pos, const Vec2f &uv, const Vec3f &normal) {
    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);
    texture_coords.push_back(uv.x);
    texture_coords.push_back(uv.y);
    normals.push_back(normal.x);
    normals.push_back(normal.y);
    normals.push_back(normal.z);
}

void GLMesh::add_face(const Vec3i &face) {
    triangles.push_back(face.x);
    triangles.push_back(face.y);
    triangles.push_back(face.z);
}

void GLMesh::compile() {
    context->glGenVertexArrays(1, &vao);
    context->glBindVertexArray(vao);

    context->glGenBuffers(1, &vertex_coord_buff);
    context->glBindBuffer(GL_ARRAY_BUFFER, vertex_coord_buff);
    context->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    context->glEnableVertexAttribArray(VSShaderLibQT::VERTEX_COORD_ATTRIB);
    context->glVertexAttribPointer(VSShaderLibQT::VERTEX_COORD_ATTRIB, 3, GL_FLOAT, 0, 0, 0);  // note: it is 3 for xyz !

    if (has_coords) {
        context->glGenBuffers(1, &texture_coord_buff);
        context->glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buff);
        context->glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(float), &texture_coords[0], GL_STATIC_DRAW);
        context->glEnableVertexAttribArray(VSShaderLibQT::TEXTURE_COORD_ATTRIB);
        context->glVertexAttribPointer(VSShaderLibQT::TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
    }

    if (has_normals) {
        context->glGenBuffers(1, &normal_buff);
        context->glBindBuffer(GL_ARRAY_BUFFER, normal_buff);
        context->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
        context->glEnableVertexAttribArray(VSShaderLibQT::NORMAL_ATTRIB);
        context->glVertexAttribPointer(VSShaderLibQT::NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
    }

    context->glGenBuffers(1, &tri_index_buff);
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_index_buff);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(unsigned int), &triangles[0], GL_STATIC_DRAW);

    context->glBindVertexArray(0);
}

GLMesh *GLMesh::plane(QOpenGLFunctionsType *context, int detailX /* = 1 */, int detailY /* = 1 */) {
    GLMesh *mesh = new GLMesh(context);
    for (int y = 0; y <= detailY; y++) {
        float t = (float)y / (float)detailY;
        for (int x = 0; x <= detailX; x++) {
            float s = (float)x / (float)detailX;
            mesh->vertices.push_back(2.f * s - 1.f);
            mesh->vertices.push_back(2.f * t - 1.f);
            mesh->vertices.push_back(0.f);
            // texture coords
            mesh->texture_coords.push_back(s);
            mesh->texture_coords.push_back(t);
            // normals
            mesh->normals.push_back(0.f);
            mesh->normals.push_back(0.f);
            mesh->normals.push_back(1.f);
            if (x < detailX && y < detailY) {
                int i = x + y * (detailX + 1);
                mesh->triangles.push_back(i);
                mesh->triangles.push_back(i + 1);
                mesh->triangles.push_back(i + detailX + 1);
                mesh->triangles.push_back(i + detailX + 1);
                mesh->triangles.push_back(i + 1);
                mesh->triangles.push_back(i + detailX + 2);
            }
        }
    }
    mesh->has_normals = true;
    mesh->has_coords = true;
    mesh->compile();
    return mesh;
}

static int cubeData[6][7] = {
    {0, 4, 2, 6, -1, 0, 0},
    {1, 3, 5, 7, +1, 0, 0},
    {0, 1, 4, 5, 0, -1, 0},
    {2, 6, 3, 7, 0, +1, 0},
    {0, 2, 1, 3, 0, 0, -1},
    {4, 5, 6, 7, 0, 0, +1}
};

void pickOctant(int input, int &x, int &y, int &z) {
    x = (input & 1) * 2 - 1;
    y = (input & 2) - 1;
    z = (input & 4) / 2 - 1;
}

GLMesh *GLMesh::cube(QOpenGLFunctionsType *context) {
    GLMesh *mesh = new GLMesh(context);

    for (int i = 0; i < 6; i++) {
        int *data = cubeData[i], v = i * 4;
        for (int j = 0; j < 4; j++) {
            int d = data[j];
            int x, y, z;
            pickOctant(d, x, y, z);
            mesh->vertices.push_back(x);
            mesh->vertices.push_back(y);
            mesh->vertices.push_back(z);
            // TODO: add coord
            int nx = cubeData[i][4], ny = cubeData[i][5], nz = cubeData[i][6];
            mesh->normals.push_back(nx);
            mesh->normals.push_back(ny);
            mesh->normals.push_back(nz);
        }
        mesh->triangles.push_back(v);
        mesh->triangles.push_back(v + 1);
        mesh->triangles.push_back(v + 2);
        mesh->triangles.push_back(v + 2);
        mesh->triangles.push_back(v + 1);
        mesh->triangles.push_back(v + 3);
    }
    mesh->has_normals = true;
    mesh->compile();
    return mesh;
}

GLMesh *GLMesh::sphere(QOpenGLFunctionsType *context, int detail /* = 6 */) {
    auto tri = [](bool flip, int a, int b, int c){ return flip ? std::tuple<int,int,int>(a, c, b) : std::tuple<int,int,int>(a, b, c); };
    auto fix = [](float x){ return x + (x - x * x) / 2.f; };
    GLMesh *mesh = new GLMesh(context);
    Indexer<float> indexer;

    for (int octant = 0; octant < 8; octant++) {
        int sx, sy, sz;
        pickOctant(octant, sx, sy, sz);
        bool flip = sx*sy*sz > 0;
        std::vector<int> data;
        for (int i = 0; i <= detail; i++) {
            // Generate a row of vertices on the surface of the sphere
            // using barycentric coordinates.
            for (int j = 0; i + j <= detail; j++) {
                float a = (float)i / (float)detail;
                float b = (float)j / (float)detail;
                float c = float(detail - i - j) / (float)detail;
                float vx = fix(a), vy = fix(b), vz = fix(c);
                double invlen = 1.0 / sqrt(double(vx*vx + vy*vy + vz*vz));
                vx *= (invlen*sx); vy *= (invlen*sy); vz *= (invlen*sz);
                // TODO: coords
                data.push_back(indexer.add(Vec3<float>(vx,vy,vz)));
            }

            // Generate triangles from this row and the previous row.
            if (i > 0) {
                for (int j = 0; i + j <= detail; j++) {
                    int a = (i - 1) * (detail + 1) + ((i - 1) - (i - 1) * (i - 1)) / 2 + j;
                    int b = i * (detail + 1) + (i - i * i) / 2 + j;
                    std::tuple<int,int,int> t = tri(flip, data[a], data[a + 1], data[b]);
                    mesh->triangles.push_back(std::get<0>(t));
                    mesh->triangles.push_back(std::get<1>(t));
                    mesh->triangles.push_back(std::get<2>(t));
                    if (i + j < detail) {
                        std::tuple<int,int,int> t2 = tri(flip, data[b], data[a + 1], data[b + 1]);
                        mesh->triangles.push_back(std::get<0>(t2));
                        mesh->triangles.push_back(std::get<1>(t2));
                        mesh->triangles.push_back(std::get<2>(t2));
                    }
                }
            }
        }
    }

    // Reconstruct the geometry from the indexer.
    for (int i = 0; i < indexer.unique.size(); i++) {
        auto &v = indexer.unique[i];
        mesh->vertices.push_back(v.x);
        mesh->vertices.push_back(v.y);
        mesh->vertices.push_back(v.z);
        // TODO: coords
        mesh->normals.push_back(v.x);
        mesh->normals.push_back(v.z);
        mesh->normals.push_back(v.y);
    }
    mesh->has_normals = true;
    mesh->compile();
    return mesh;
}
