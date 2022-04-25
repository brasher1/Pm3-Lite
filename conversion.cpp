// conversion.cpp   Converts internal format to/from triton database
#include <conversion.h>
/*
void CnvRDMPtgCone(SoCone *s, struct cone *c)
{
    s->height = c->cone_height;
    s->bottomRadius = c->cone_radius;
}

void CnvPtgRDMCone(struct cone *c, SoCone *s)
{
    c->cone_height = s->height.getValue();
    c->cone_radius = s->bottomRadius.getValue();
}

void CnvRDMPtgBox(SoCube *s, struct box *b)
{
    s->width = b->box_width;
    s->height = b->box_height;
    s->depth = b->box_depth;
}

void CnvPtgRDMBox(struct box *b, SoCube *s)
{
    b->box_width = s->width.getValue();
    b->box_height = s->height.getValue();
    b->box_depth = s->depth.getValue();
}

void CnvRDMPtgSphere(SoSphere *s, struct sphere *sp)
{
    s->radius = sp->sph_diameter / 2;
}

void CnvPtgRDMSphere(struct sphere *sp, SoSphere *s)
{
    sp->sph_diameter = (s->radius.getValue()) * 2.0;
}

void CnvRDMPtgCylinder(SoCylinder *s, struct cylinder *c)
{
    s->radius = c->cyl_radius;
    s->height = c->cyl_height;
}

void CnvPtgRDMCylinder(struct cylinder *c, SoCylinder *s)
{
    c->cyl_radius = s->radius.getValue();
    c->cyl_height = s->height.getValue();
}

void CnvPtgRDMPyramid(struct pyramid *p, Pyramid *s)
{
    p->pyr_bott_width = s->baseWidth.getValue();
    p->pyr_bott_length = s->baseDepth.getValue();
    p->pyr_height = s->height.getValue();
    p->pyr_top_width = s->topWidth.getValue();
    p->pyr_top_length = s->topDepth.getValue();
    p->pyr_width_offset = s->widthOffset.getValue();
    p->pyr_length_offset = s->depthOffset.getValue();
}

void CnvRDMPtgPyramid(Pyramid *s, struct pyramid *p)
{
    s->baseWidth = p->pyr_bott_width;
    s->baseDepth = p->pyr_bott_length;
    s->topWidth = p->pyr_top_width;
    s->topDepth = p->pyr_top_length;
    s->height = p->pyr_height;
    s->widthOffset = p->pyr_width_offset;
    s->depthOffset = p->pyr_length_offset;
}

void CnvPtgRDMSnout(struct snout *p, Snout *s)
{
    p->snt_bott_radius = s->baseRadius.getValue();
    p->snt_height = s->height.getValue();
    p->snt_top_radius = s->topRadius.getValue();
    p->snt_x_offset = s->widthOffset.getValue();
    p->snt_y_offset = s->depthOffset.getValue();
}

void CnvRDMPtgSnout(Snout *s, struct snout *p)
{
    s->baseRadius = p->snt_bott_radius;
    s->topRadius = p->snt_top_radius;
    s->height = p->snt_height;
    s->widthOffset = p->snt_x_offset;
    s->depthOffset = p->snt_y_offset;
    p->snt_y_offset = s->depthOffset.getValue();
}

void CnvPtgRDMCircTorus(struct cir_torus *c, CircTorus *s)
{
    c->ctor_inside_radius = s->innerRadius.getValue();
    c->ctor_angle = s->angle.getValue();
    c->ctor_outside_radius = s->outerRadius.getValue();
}

void CnvRDMPtgCircTorus(CircTorus *s, struct cir_torus *c)
{
    s->innerRadius = c->ctor_inside_radius;
    s->outerRadius = c->ctor_outside_radius;
    s->angle = c->ctor_angle;
}

void CnvPtgRDMRectTorus(struct rect_torus *r, RectTorus *s)
{
    r->rtor_inside_radius = s->innerRadius.getValue();
    r->rtor_angle = s->angle.getValue();
    r->rtor_outside_radius = s->outerRadius.getValue();
    r->rtor_height = s->height.getValue();	
}

void CnvRDMPtgRectTorus(RectTorus *s, struct rect_torus *r)
{
    s->innerRadius = r->rtor_inside_radius;
    s->outerRadius = r->rtor_outside_radius;
    s->angle = r->rtor_angle;
    s->height = r->rtor_height;
}
*/