<?php
/**
 * @file
 * Views for customizing displays.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_custom_product_views_default_views() {
  $views = array();

  // Line item view for the order view page.
  $view = new view();
  $view->name = 'commerce_customizing_summary';
  $view->description = 'Customizing summary displayed during checkout.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Customizing summary';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Customizing summary';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'line_item_title_1',
      'rendered' => 0,
      'rendered_strip' => 0,
    ),
    1 => array(
      'field' => 'product_customization_description',
      'rendered' => 0,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'line_item_title_1' => 'line_item_title_1',
    'product_customization_description' => 'product_customization_description',
    'rendered_entity' => 'rendered_entity',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'line_item_title_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'product_customization_description' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Commerce Order: Referenced line items */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['commerce_parent_line_item_target_id']['id'] = 'commerce_parent_line_item_target_id';
  $handler->display->display_options['relationships']['commerce_parent_line_item_target_id']['table'] = 'field_data_commerce_parent_line_item';
  $handler->display->display_options['relationships']['commerce_parent_line_item_target_id']['field'] = 'commerce_parent_line_item_target_id';
  $handler->display->display_options['relationships']['commerce_parent_line_item_target_id']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_parent_line_item_target_id']['label'] = 'Parent Line Item';
  $handler->display->display_options['relationships']['commerce_parent_line_item_target_id']['required'] = TRUE;
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title_1']['id'] = 'line_item_title_1';
  $handler->display->display_options['fields']['line_item_title_1']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title_1']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title_1']['relationship'] = 'commerce_parent_line_item_target_id';
  $handler->display->display_options['fields']['line_item_title_1']['label'] = '';
  $handler->display->display_options['fields']['line_item_title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['line_item_title_1']['element_label_colon'] = FALSE;
  /* Field: Commerce Line Item: Show the customizing description */
  $handler->display->display_options['fields']['product_customization_description']['id'] = 'product_customization_description';
  $handler->display->display_options['fields']['product_customization_description']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['product_customization_description']['field'] = 'product_customization_description';
  $handler->display->display_options['fields']['product_customization_description']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['product_customization_description']['label'] = '';
  $handler->display->display_options['fields']['product_customization_description']['exclude'] = TRUE;
  $handler->display->display_options['fields']['product_customization_description']['element_label_colon'] = FALSE;
  /* Field: Commerce Line Item: Rendered Commerce Line item */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_commerce_line_item';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'customizing_review';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Sort criterion: Commerce Line Item: Sort customization line items */
  $handler->display->display_options['sorts']['product_customization_order']['id'] = 'product_customization_order';
  $handler->display->display_options['sorts']['product_customization_order']['table'] = 'commerce_line_item';
  $handler->display->display_options['sorts']['product_customization_order']['field'] = 'product_customization_order';
  $handler->display->display_options['sorts']['product_customization_order']['relationship'] = 'commerce_line_items_line_item_id';
  /* Contextual filter: Commerce Order: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Commerce Line Item: Line item is of a product line item type */
  $handler->display->display_options['filters']['product_line_item_type']['id'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['product_line_item_type']['field'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['filters']['product_line_item_type']['value'] = '0';
  $handler->display->display_options['filters']['product_line_item_type']['group'] = 0;
  $translatables['commerce_customizing_summary'] = array(
    t('Defaults'),
    t('Shopping cart'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Line items referenced by commerce_line_items'),
    t('Parent Line Item'),
    t('All'),
  );

  $views[$view->name] = $view;

  return $views;
}
