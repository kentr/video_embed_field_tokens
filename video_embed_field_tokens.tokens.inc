<?php

/**
 * @file
 * Token callbacks for the video_embed_field_tokens module.
 */

/**
 * Implements hook_token_info().
 *
 * @ingroup video_embed_field_tokens
 */
function video_embed_field_tokens_token_info() {

  $info['tokens']['node'] = [
    'field_video_embed_player_url' => [
      'name' => t('Video Embed Player URL'),
      'description' => t("The URL of the embedded player for Video Embed field.  Can be used with Metatag, such as for Twitter Media player URL."),
    ],
    'field_video_embed_player_width' => [
      'name' => t('Video Embed Player Width'),
      'description' => t("The width of the embedded player for Video Embed field.  Can be used with Metatag, such as for Twitter Media player width."),
    ],
    'field_video_embed_player_height' => [
      'name' => t('Video Embed Player Height'),
      'description' => t("The height of the embedded player for Video Embed field.  Can be used with Metatag, such as for Twitter Media player height."),
    ],

  ];

  return $info;
}

/**
 * Implements hook_tokens().
 *
 * @ingroup video_embed_field_tokens
 */
function video_embed_field_tokens_tokens($type, $tokens, array $data = [], array $options = []) {
  $replacements = [];
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    // Get rendered embed code for parsing.
    $vef_renderable = field_view_field('node', $node, 'field_video_embed');
    $vef_html = render($vef_renderable);

    $dom = new DOMDocument();
    $dom->loadHTML($vef_html);
    $iframe_node = $dom->getElementsByTagName('iframe')->item(0);

    // Replace tokens.
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'field_video_embed_player_url':
          // Testing with Twitter Card Validator indicates that
          // `twitter:player` URL needs to be URL-decoded.
          $replacements[$original] = urldecode($iframe_node->getAttribute('src'));
          break;

        case 'field_video_embed_player_width':
          $replacements[$original] = $iframe_node->getAttribute('width');
          break;

        case 'field_video_embed_player_height':
          $replacements[$original] = $iframe_node->getAttribute('height');
          break;
      }
    }
  }
  return $replacements;
}
